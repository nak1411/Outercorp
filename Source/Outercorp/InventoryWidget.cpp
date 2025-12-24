// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryListRowWidget.h"
#include "InventoryListHeaderWidget.h"
#include "InventoryColumnSettings.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/PanelSlot.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Input/Reply.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"

/**
 * Input processor that captures clicks outside the inventory widget
 */
class FInventoryInputProcessor : public IInputProcessor
{
public:
	FInventoryInputProcessor(UInventoryWidget* InInventoryWidget)
		: InventoryWidget(InInventoryWidget)
	{
	}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		if (!InventoryWidget.IsValid())
		{
			return false;
		}

		// Only handle left mouse button for deselecting
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			// Don't deselect if there's an open context menu - let the context menu handle its own clicks
			// The context menu will close itself when clicked outside, and then we can deselect
			if (InventoryWidget->SlotWidgets.Num() > 0 && InventoryWidget->SlotWidgets[0])
			{
				UUserWidget* OpenContextMenu = InventoryWidget->SlotWidgets[0]->GetCurrentContextMenu();
				if (OpenContextMenu && OpenContextMenu->IsInViewport())
				{
					// Context menu is open - don't clear selections, let the menu handle the click
					return false;
				}
			}

			// Get the widget under the cursor
			FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(MouseEvent.GetScreenSpacePosition(), SlateApp.GetInteractiveTopLevelWindows());

			if (WidgetPath.IsValid())
			{
				// Check if the click is inside the inventory widget
				TSharedPtr<SWidget> InventorySlateWidget = InventoryWidget->TakeWidget();

				bool bClickedInsideInventory = false;
				for (int32 i = 0; i < WidgetPath.Widgets.Num(); ++i)
				{
					// Check if clicked in inventory
					if (WidgetPath.Widgets[i].Widget == InventorySlateWidget)
					{
						bClickedInsideInventory = true;
						break;
					}
				}

				// If clicked outside inventory, deselect all items
				if (!bClickedInsideInventory)
				{
					// Clear grid selections
					if (InventoryWidget->SlotWidgets.Num() > 0 && InventoryWidget->SlotWidgets[0])
					{
						InventoryWidget->SlotWidgets[0]->ClearAllSelections();
					}

					// Clear list row selections
					UInventoryListRowWidget::ClearAllRowSelections();
				}
			}
		}

		// Don't consume the event - let it pass through
		return false;
	}

private:
	TWeakObjectPtr<UInventoryWidget> InventoryWidget;
};

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize grid tracking variables
	CachedGridSize = FVector2D::ZeroVector;
	CurrentVisibleColumns = 0;
	CurrentVisibleRows = 0;
	CurrentVisibleSlots = 0;
	PendingColumnCount = 0;
	bWaitingForGeometry = false;

	// Set up click capture for deselecting items when clicking outside
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			SetupTimerHandle,
			this,
			&UInventoryWidget::SetupClickCapture,
			0.1f,  // 100ms delay
			false
		);
	}

	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: Starting button binding"));

	// Bind button events
	if (CloseButton)
	{
		UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: CloseButton found, binding OnClicked"));
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct: CloseButton is NULL! Make sure you have a Button named 'CloseButton' in your widget Blueprint with BindWidget or BindWidgetOptional"));
	}

	if (SortByNameButton)
	{
		SortByNameButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortByNameClicked);
	}

	if (SortByRarityButton)
	{
		SortByRarityButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortByRarityClicked);
	}

	if (SearchText)
	{
		SearchText->OnTextChanged.AddDynamic(this, &UInventoryWidget::OnSearchTextChanged);
	}

	if (GridViewButton)
	{
		GridViewButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnGridViewButtonClicked);
	}

	if (ListViewButton)
	{
		ListViewButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnListViewButtonClicked);
	}

	// Set up the ListView entry widget class
	if (ItemListView && ListRowWidgetClass)
	{
		// Note: The entry widget class should also be set in the Blueprint,
		// but we set it here as well to ensure it's configured
		UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: Setting ListView entry widget class"));
	}
	else if (ItemListView && !ListRowWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct: ListView exists but ListRowWidgetClass is not set!"));
	}

	// Create shared column settings
	ColumnSettings = NewObject<UInventoryColumnSettings>(this);

	// Bind header column click events and set column settings
	if (ListViewHeader)
	{
		ListViewHeader->OnColumnHeaderClicked.AddDynamic(this, &UInventoryWidget::OnColumnHeaderClicked);
		ListViewHeader->SetColumnSettings(ColumnSettings);
		UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: Bound to ListViewHeader column click events"));
	}

	// Initialize to grid view by default
	SetViewMode(false);
}

void UInventoryWidget::NativeDestruct()
{
	// Clean up timers
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SetupTimerHandle);
		World->GetTimerManager().ClearTimer(ReflowDebounceTimerHandle);
	}

	// Remove input processor
	if (InputProcessor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
		InputProcessor.Reset();
	}

	bIsClickCaptureActive = false;

	// Delegates are automatically cleaned up when the widget is destroyed
	// No need to manually unbind

	Super::NativeDestruct();
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// If we're waiting for geometry and slots haven't been created yet, try to create them
	if (bWaitingForGeometry && SlotWidgets.Num() == 0)
	{
		FVector2D AvailableSize = FVector2D::ZeroVector;
		if (ItemScrollBox)
		{
			AvailableSize = ItemScrollBox->GetCachedGeometry().GetLocalSize();
		}
		else if (ItemGrid)
		{
			AvailableSize = ItemGrid->GetCachedGeometry().GetLocalSize();
		}

		// Once we have valid geometry, create the slots
		if (AvailableSize.X > 0 && AvailableSize.Y > 0)
		{
			CreateSlotWidgets();
			bWaitingForGeometry = false;
		}
	}

	// Dynamically reflow items when window is resized (debounced)
	if (ItemScrollBox)
	{
		FVector2D CurrentSize = ItemScrollBox->GetCachedGeometry().GetLocalSize();
		if (!CachedGridSize.Equals(CurrentSize, 1.0f))
		{
			bool bWidthChanged = !FMath::IsNearlyEqual(CachedGridSize.X, CurrentSize.X, 1.0f);
			bool bHeightChanged = !FMath::IsNearlyEqual(CachedGridSize.Y, CurrentSize.Y, 1.0f);

			CachedGridSize = CurrentSize;

			// Calculate how many columns can fit in the current width
			int32 NewColumnCount = CalculateColumnsFromWidth(CurrentSize.X);

			// Schedule debounced reflow if width changed (column count may have changed)
			// or if height changed (need to compress items to fit new visible area)
			bool bNeedsReflow = (bWidthChanged && NewColumnCount != CurrentVisibleColumns && NewColumnCount > 0);
			bool bNeedsCompress = bHeightChanged;

			if (bNeedsReflow || bNeedsCompress)
			{
				if (bNeedsReflow)
				{
					PendingColumnCount = NewColumnCount;
				}

				// Cancel any existing timer and start a new one
				if (UWorld* World = GetWorld())
				{
					World->GetTimerManager().ClearTimer(ReflowDebounceTimerHandle);
					World->GetTimerManager().SetTimer(
						ReflowDebounceTimerHandle,
						this,
						&UInventoryWidget::ExecuteDebouncedReflow,
						0.2f,  // 200ms delay after resize stops
						false
					);
				}
			}
		}
	}
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
	// Handle ESC or I key to close inventory
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::I)
	{
		CloseInventory();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// If clicking in the inventory widget (but not on a slot/row), deselect all items
	// The slots and rows will handle their own clicks and prevent this from being called
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && InventoryComponent)
	{
		// Clear grid slot selections
		if (SlotWidgets.Num() > 0 && SlotWidgets[0])
		{
			SlotWidgets[0]->ClearAllSelections();
		}

		// Clear list row selections
		UInventoryListRowWidget::ClearAllRowSelections();
	}
	// Handle right-click in list view to show empty context menu
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsListView)
	{
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			UE_LOG(LogTemp, Log, TEXT("Right-clicked in empty area of list view at position (%f, %f)"), MousePosition.X, MousePosition.Y);
			OnEmptyAreaRightClicked(MousePosition);

			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryWidget::InitializeInventory(UInventoryComponent *InInventoryComponent)
{
	if (!InInventoryComponent)
	{
		return;
	}

	InventoryComponent = InInventoryComponent;

	// Bind to inventory events
	InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	InventoryComponent->OnInventoryCapacityChanged.AddDynamic(this, &UInventoryWidget::OnCapacityChanged);

	// Try to create slot widgets immediately
	CreateSlotWidgets();

	// If slots weren't created (no geometry yet), flag to try on next tick
	if (SlotWidgets.Num() == 0)
	{
		bWaitingForGeometry = true;
	}
	else
	{
		// Initial refresh if slots were created
		RefreshInventory();
	}
}

void UInventoryWidget::RefreshInventory()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Update the tracking set with current state
	PreviouslyOccupiedSlots.Empty();
	for (int32 i = 0; i < InventoryComponent->MaxSlots; ++i)
	{
		FInventoryItem Item = InventoryComponent->GetItemAtSlot(i);
		if (Item.IsValid())
		{
			PreviouslyOccupiedSlots.Add(i);
		}
	}

	// Refresh all slots
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		RefreshSlot(i);
	}

	// Update capacity display
	UpdateCapacityDisplay();

	// Update scrollbar visibility based on items
	UpdateScrollbarVisibility();

	// Also refresh list view if we're in list view mode
	if (bIsListView)
	{
		if (ListViewRowContainer)
		{
			PopulateTableView();
		}
		else if (ItemListView)
		{
			PopulateListView();
		}
	}
}

void UInventoryWidget::RefreshSlot(int32 SlotIndex)
{
	if (!InventoryComponent || !SlotWidgets.IsValidIndex(SlotIndex))
	{
		return;
	}

	FInventoryItem Item = InventoryComponent->GetItemAtSlot(SlotIndex);

	if (SlotWidgets[SlotIndex])
	{
		SlotWidgets[SlotIndex]->SetItem(Item);

		// Show/hide based on whether slot has an item and passes filter
		bool bHasItem = Item.IsValid();
		bool bPassesFilter = !bHasItem || PassesFilter(Item);

		// Empty slots must be Visible to accept drag-drop events
		// Filtered slots are completely collapsed
		if (!bPassesFilter)
		{
			SlotWidgets[SlotIndex]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			// Both empty and filled slots need to be Visible to receive drag-drop events
			SlotWidgets[SlotIndex]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryWidget::UpdateCapacityDisplay()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Update weight display
	if (WeightText)
	{
		float CurrentWeight = InventoryComponent->GetCurrentWeight();
		float MaxWeight = InventoryComponent->MaxWeight;

		if (MaxWeight > 0.0f)
		{
			WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f kg"), CurrentWeight, MaxWeight)));

			if (WeightProgressBar)
			{
				WeightProgressBar->SetPercent(CurrentWeight / MaxWeight);
			}
		}
		else
		{
			WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f kg"), CurrentWeight)));

			if (WeightProgressBar)
			{
				WeightProgressBar->SetPercent(0.0f);
			}
		}
	}

	// Update capacity display
	if (CapacityText)
	{
		int32 OccupiedSlots = InventoryComponent->GetOccupiedSlots();
		int32 MaxSlots = InventoryComponent->MaxSlots;
		CapacityText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), OccupiedSlots, MaxSlots)));
	}
}

void UInventoryWidget::CloseInventory()
{
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::CloseInventory: Broadcasting close event"));
	// Just broadcast the close event - let the owning character handle cleanup
	OnInventoryClosed.Broadcast();
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::CloseInventory: Broadcast complete"));
}

void UInventoryWidget::OnInventoryUpdated(int32 SlotIndex, const FInventoryItem &Item)
{
	// Just refresh the specific slot - all slots are already created
	RefreshSlot(SlotIndex);
	UpdateCapacityDisplay();

	// Update scrollbar visibility when items change
	UpdateScrollbarVisibility();

	// Check if this is a NEWLY occupied slot (for split/add operations)
	bool bWasPreviouslyEmpty = !PreviouslyOccupiedSlots.Contains(SlotIndex);
	bool bIsNowOccupied = Item.IsValid();

	// Update the tracking set
	if (bIsNowOccupied)
	{
		PreviouslyOccupiedSlots.Add(SlotIndex);
	}
	else
	{
		PreviouslyOccupiedSlots.Remove(SlotIndex);
	}

	// Also refresh list view if we're in list view mode
	if (bIsListView)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnInventoryUpdated: Refreshing list view because we're in list mode"));
		if (ListViewRowContainer)
		{
			PopulateTableView();
		}
		else if (ItemListView)
		{
			PopulateListView();
		}
	}

	// Don't auto-scroll - it causes issues with view jumping around
	// User can manually compress if needed
}

void UInventoryWidget::OnCapacityChanged(int32 NewCapacity)
{
	// Recreate slot widgets if capacity changed
	CreateSlotWidgets();
	RefreshInventory();
}

void UInventoryWidget::OnCloseButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::OnCloseButtonClicked: X button clicked"));
	CloseInventory();
}

void UInventoryWidget::OnSortByNameClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->SortInventory(true);
	}
}

void UInventoryWidget::OnSortByRarityClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->SortInventory(false);
	}
}

void UInventoryWidget::OnSearchTextChanged(const FText &Text)
{
	CurrentFilter = Text.ToString();
	RefreshInventory();
}

void UInventoryWidget::ApplyClippingSettings()
{
	// Configure outer ScrollBox for vertical scrolling
	if (ItemScrollBox)
	{
		ItemScrollBox->SetClipping(EWidgetClipping::ClipToBounds);
		ItemScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
		ItemScrollBox->SetAllowOverscroll(false);
		ItemScrollBox->SetOrientation(EOrientation::Orient_Vertical);

		UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: Vertical ScrollBox configured"));

		// Update scrollbar visibility based on current items
		UpdateScrollbarVisibility();
	}

	// Configure inner ScrollBox for horizontal scrolling
	if (HorizontalScrollBox)
	{
		HorizontalScrollBox->SetClipping(EWidgetClipping::ClipToBounds);
		HorizontalScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never); // Let vertical handle mouse wheel
		HorizontalScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		HorizontalScrollBox->SetAllowOverscroll(false);
		HorizontalScrollBox->SetOrientation(EOrientation::Orient_Horizontal);
		HorizontalScrollBox->SetAlwaysShowScrollbar(false);

		UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: Horizontal ScrollBox configured"));
	}

	// Force the UniformGridPanel to use exact slot sizes without compression
	if (ItemGrid)
	{
		// Set slot padding and force minimum sizes
		ItemGrid->SetSlotPadding(SlotPadding);
		ItemGrid->SetMinDesiredSlotWidth(SlotSize);
		ItemGrid->SetMinDesiredSlotHeight(SlotSize);

		// Calculate the fixed width needed for the grid based on actual columns used
		// Use CurrentVisibleColumns if set, otherwise fall back to GridColumns
		int32 ColumnsForWidth = (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns;
		float GridWidth = (ColumnsForWidth * SlotSize) + ((ColumnsForWidth + 1) * SlotPadding.Left);

		// Check if ItemGrid is already wrapped in a SizeBox at Blueprint level
		UPanelSlot* GridSlot = ItemGrid->Slot;
		USizeBox* ParentSizeBox = GridSlot ? Cast<USizeBox>(GridSlot->Parent) : nullptr;

		if (ParentSizeBox)
		{
			// Already in a SizeBox, set fixed width to prevent compression
			ParentSizeBox->SetWidthOverride(GridWidth);
			ParentSizeBox->SetMinDesiredWidth(GridWidth);
			ParentSizeBox->SetMaxDesiredWidth(GridWidth);

			// Remove height override to let content determine height naturally
			ParentSizeBox->ClearHeightOverride();
			ParentSizeBox->SetMinDesiredHeight(0.0f);

			UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: SizeBox - Fixed width: %f (%d columns), Natural height"), GridWidth, ColumnsForWidth);
		}
		else if (!GridSizeBox)
		{
			// Create a SizeBox wrapper at runtime to enforce fixed width
			GridSizeBox = NewObject<USizeBox>(this);
			GridSizeBox->SetWidthOverride(GridWidth);
			GridSizeBox->SetMinDesiredWidth(GridWidth);
			GridSizeBox->SetMaxDesiredWidth(GridWidth);
			// Don't set height override - let grid determine its natural height

			UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: Created SizeBox wrapper - Fixed width: %f (%d columns)"), GridWidth, ColumnsForWidth);
		}

		UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: Grid slot size %f x %f, fixed width: %f for %d columns"), SlotSize, SlotSize, GridWidth, ColumnsForWidth);
	}
}

void UInventoryWidget::EnableHorizontalScrolling()
{
	if (!ItemScrollBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnableHorizontalScrolling: ItemScrollBox is null"));
		return;
	}

	// Access the underlying Slate widget to enable horizontal scrolling
	TSharedPtr<SWidget> SlateWidget = ItemScrollBox->GetCachedWidget();
	if (!SlateWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EnableHorizontalScrolling: Could not get Slate widget"));
		return;
	}

	TSharedPtr<SScrollBox> SlateScrollBox = StaticCastSharedPtr<SScrollBox>(SlateWidget);
	if (SlateScrollBox.IsValid())
	{
		// Enable both vertical and horizontal scrolling
		SlateScrollBox->SetScrollBarRightClickDragAllowed(true);
		SlateScrollBox->SetAllowOverscroll(EAllowOverscroll::No);

		UE_LOG(LogTemp, Log, TEXT("EnableHorizontalScrolling: Horizontal scrolling enabled via Slate"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnableHorizontalScrolling: Failed to cast to SScrollBox"));
	}
}

void UInventoryWidget::CreateSlotWidgets()
{
	if (!ItemGrid || !SlotWidgetClass || !InventoryComponent)
	{
		return;
	}

	// Only create slots once - don't recreate on resize
	if (SlotWidgets.Num() > 0)
	{
		return;
	}

	// Always use MaxPreCreatedColumns to create slots
	// This allows the grid to expand when window is resized wider
	int32 ColumnsToUse = MaxPreCreatedColumns;

	// Calculate total rows needed based on max columns
	int32 RowsNeeded = FMath::CeilToInt(static_cast<float>(InventoryComponent->MaxSlots) / static_cast<float>(ColumnsToUse));
	RowsNeeded = FMath::Min(RowsNeeded, MaxPreCreatedRows);

	UE_LOG(LogTemp, Log, TEXT("CreateSlotWidgets: Pre-creating maximum grid %d x %d for inventory capacity of %d"),
		ColumnsToUse, RowsNeeded, InventoryComponent->MaxSlots);

	// Pre-create all slots in a fixed grid layout
	int32 TotalSlots = ColumnsToUse * RowsNeeded;

	UE_LOG(LogTemp, Log, TEXT("CreateSlotWidgets: Creating grid %d x %d = %d slots for inventory capacity of %d"),
		ColumnsToUse, RowsNeeded, TotalSlots, InventoryComponent->MaxSlots);

	for (int32 i = 0; i < TotalSlots; ++i)
	{
		UInventorySlotWidget *SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			SlotWidget->SetSlotIndex(i);
			SlotWidget->SetInventoryComponent(InventoryComponent);

			int32 Row = i / ColumnsToUse;
			int32 Column = i % ColumnsToUse;

			// Wrap in a SizeBox to enforce the exact slot size and prevent compression
			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(SlotSize);
			SizeBox->SetHeightOverride(SlotSize);
			SizeBox->SetMinDesiredWidth(SlotSize);
			SizeBox->SetMinDesiredHeight(SlotSize);
			SizeBox->SetMaxDesiredWidth(SlotSize);
			SizeBox->SetMaxDesiredHeight(SlotSize);
			SizeBox->AddChild(SlotWidget);

			// Add to grid
			ItemGrid->AddChildToUniformGrid(SizeBox, Row, Column);
			SlotWidgets.Add(SlotWidget);
		}
	}

	// Store the column count used
	CurrentVisibleColumns = ColumnsToUse;
	CurrentVisibleRows = RowsNeeded;

	// Apply clipping settings with the actual column count
	ApplyClippingSettings();

	// If we have valid geometry, calculate the actual visible columns based on width
	if (ItemScrollBox)
	{
		FVector2D CurrentSize = ItemScrollBox->GetCachedGeometry().GetLocalSize();
		if (CurrentSize.X > 0.0f)
		{
			int32 CalculatedColumns = CalculateColumnsFromWidth(CurrentSize.X);
			if (CalculatedColumns > 0)
			{
				CurrentVisibleColumns = CalculatedColumns;
				UE_LOG(LogTemp, Warning, TEXT("CreateSlotWidgets: Adjusted visible columns to %d based on width %.1f"), CurrentVisibleColumns, CurrentSize.X);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateSlotWidgets: Grid created with %d pre-created columns, %d visible columns"), ColumnsToUse, CurrentVisibleColumns);

	// Update visibility after creating slots
	UpdateSlotVisibility();
}

void UInventoryWidget::RecalculateGridLayout(const FVector2D& AvailableSize)
{
	// No longer needed - all slots are pre-created
	// This function is kept for compatibility but does nothing
}

void UInventoryWidget::UpdateSlotVisibility()
{
	if (!InventoryComponent)
	{
		return;
	}

	int32 MaxSlots = InventoryComponent->MaxSlots;

	// Update visibility for all slot widgets
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		if (SlotWidgets[i])
		{
			// Slots beyond the inventory capacity should be completely hidden
			if (i >= MaxSlots)
			{
				SlotWidgets[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				// Let RefreshSlot handle the visibility based on item and filter
				RefreshSlot(i);
			}
		}
	}
}

bool UInventoryWidget::PassesFilter(const FInventoryItem &Item) const
{
	if (CurrentFilter.IsEmpty() || !Item.IsValid())
	{
		return true;
	}

	// Check if item name contains filter text
	FString ItemName = Item.ItemData->ItemName.ToString();
	return ItemName.Contains(CurrentFilter, ESearchCase::IgnoreCase);
}

void UInventoryWidget::SetupClickCapture()
{
	if (bIsClickCaptureActive)
	{
		return;
	}

	// Create and register input processor for detecting clicks outside
	// Use a very low priority (999) so other widgets process input first
	if (FSlateApplication::IsInitialized())
	{
		InputProcessor = MakeShared<FInventoryInputProcessor>(this);
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 999);
		bIsClickCaptureActive = true;

		UE_LOG(LogTemp, Log, TEXT("Inventory click capture activated"));
	}
}

void UInventoryWidget::UpdateScrollbarVisibility()
{
	if (!ItemScrollBox || !InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility: ItemScrollBox or InventoryComponent is null"));
		return;
	}

	// Find the first and last occupied slots
	int32 FirstOccupiedSlot = -1;
	int32 LastOccupiedSlot = -1;

	for (int32 i = 0; i < InventoryComponent->MaxSlots; ++i)
	{
		FInventoryItem Item = InventoryComponent->GetItemAtSlot(i);
		if (Item.IsValid())
		{
			if (FirstOccupiedSlot == -1)
			{
				FirstOccupiedSlot = i;
			}
			LastOccupiedSlot = i;
		}
	}

	// If no items, hide scrollbar and disable scrolling
	if (FirstOccupiedSlot == -1)
	{
		ItemScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		ItemScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
		UE_LOG(LogTemp, Log, TEXT("UpdateScrollbarVisibility: No items, hiding scrollbar and disabling scroll"));
		return;
	}

	int32 ColumnsToUse = (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns;

	// Calculate the first and last rows containing items
	int32 FirstRowWithItem = FirstOccupiedSlot / ColumnsToUse;
	int32 LastRowWithItem = LastOccupiedSlot / ColumnsToUse;

	// Get the available height of the scroll box
	float AvailableHeight = ItemScrollBox->GetCachedGeometry().GetLocalSize().Y;
	float RowHeight = SlotSize + (SlotPadding.Top + SlotPadding.Bottom);

	UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility DEBUG: LastOccupiedSlot=%d, ColumnsToUse=%d, LastRowWithItem=%d, AvailableHeight=%.1f, RowHeight=%.1f"),
		LastOccupiedSlot, ColumnsToUse, LastRowWithItem, AvailableHeight, RowHeight);

	// If geometry is not valid yet, force scrollbar to be visible if there are items beyond first few rows
	if (AvailableHeight <= 0.0f)
	{
		// No valid geometry yet - be conservative and show scrollbar if there are many items
		if (LastRowWithItem > 3)
		{
			ItemScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
			ItemScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
			UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility: No valid geometry, but showing scrollbar due to LastRow=%d"), LastRowWithItem);
		}
		return;
	}

	// Calculate how many rows can fit in the visible area
	int32 VisibleRows = FMath::FloorToInt(AvailableHeight / RowHeight);

	// Calculate the total number of rows needed to display all items
	// We need to check from row 0 to the last row with items
	int32 TotalRowsNeeded = LastRowWithItem + 1;

	UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility DEBUG: TotalRowsNeeded=%d, VisibleRows=%d"), TotalRowsNeeded, VisibleRows);

	// Show scrollbar if the content needs more rows than can fit in the viewport
	if (TotalRowsNeeded > VisibleRows)
	{
		ItemScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
		ItemScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
		UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility: SHOWING SCROLLBAR - Items need scrolling (TotalRows: %d, VisibleRows: %d, LastRow: %d)"),
			TotalRowsNeeded, VisibleRows, LastRowWithItem);
	}
	else
	{
		ItemScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		ItemScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
		UE_LOG(LogTemp, Warning, TEXT("UpdateScrollbarVisibility: HIDING SCROLLBAR - All items visible (TotalRows: %d, VisibleRows: %d, LastRow: %d)"),
			TotalRowsNeeded, VisibleRows, LastRowWithItem);
	}
}

void UInventoryWidget::CompressItems()
{
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CompressItems: InventoryComponent is null"));
		return;
	}

	int32 ColumnsToUse = (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns;

	UE_LOG(LogTemp, Warning, TEXT("CompressItems: Starting compression with %d columns"), ColumnsToUse);

	// Build a list of all items with their current slot indices
	TArray<TPair<int32, FInventoryItem>> OccupiedSlots;

	for (int32 i = 0; i < InventoryComponent->MaxSlots; ++i)
	{
		FInventoryItem Item = InventoryComponent->GetItemAtSlot(i);
		if (Item.IsValid())
		{
			OccupiedSlots.Add(TPair<int32, FInventoryItem>(i, Item));
		}
	}

	if (OccupiedSlots.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("CompressItems: No items to compress"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CompressItems: Found %d occupied slots"), OccupiedSlots.Num());

	// Calculate visible area based on current window size
	float AvailableHeight = 0.0f;
	if (ItemScrollBox)
	{
		AvailableHeight = ItemScrollBox->GetCachedGeometry().GetLocalSize().Y;
	}

	int32 MaxVisibleSlots = InventoryComponent->MaxSlots;

	// If we have valid geometry, calculate how many slots fit in visible area
	if (AvailableHeight > 0.0f)
	{
		float RowHeight = SlotSize + (SlotPadding.Top + SlotPadding.Bottom);
		int32 VisibleRows = FMath::FloorToInt(AvailableHeight / RowHeight);
		MaxVisibleSlots = ColumnsToUse * VisibleRows;
		MaxVisibleSlots = FMath::Min(MaxVisibleSlots, InventoryComponent->MaxSlots);

		UE_LOG(LogTemp, Warning, TEXT("CompressItems: Visible area can fit %d rows x %d columns = %d slots"),
			VisibleRows, ColumnsToUse, MaxVisibleSlots);
	}

	// Move each item to the earliest available slot within visible area
	// Process from the beginning to ensure we fill slots in order
	int32 NextTargetSlot = 0;

	for (const TPair<int32, FInventoryItem>& SlotPair : OccupiedSlots)
	{
		int32 CurrentSlot = SlotPair.Key;

		// If the item is already in the target slot, just advance to next
		if (CurrentSlot == NextTargetSlot)
		{
			NextTargetSlot++;
			continue;
		}

		// Move the item from CurrentSlot to NextTargetSlot
		if (NextTargetSlot < InventoryComponent->MaxSlots)
		{
			// Use the inventory component's MoveItem function to properly handle the move
			bool bMoveSuccess = InventoryComponent->MoveItem(CurrentSlot, NextTargetSlot);

			if (bMoveSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("CompressItems: Moved item from slot %d to slot %d"), CurrentSlot, NextTargetSlot);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("CompressItems: Failed to move item from slot %d to slot %d"), CurrentSlot, NextTargetSlot);
			}

			NextTargetSlot++;
		}
	}

	// Refresh the entire inventory display
	RefreshInventory();

	// Update scrollbar visibility after compression
	UpdateScrollbarVisibility();

	UE_LOG(LogTemp, Warning, TEXT("CompressItems: Compression complete, items now occupy slots 0-%d"), NextTargetSlot - 1);
}

void UInventoryWidget::ScrollToSlot(int32 TargetSlotIndex)
{
	if (!ItemScrollBox || !InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScrollToSlot: ItemScrollBox or InventoryComponent is null"));
		return;
	}

	if (TargetSlotIndex < 0 || TargetSlotIndex >= InventoryComponent->MaxSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScrollToSlot: Invalid slot index %d"), TargetSlotIndex);
		return;
	}

	// First, update scrollbar visibility to ensure it's shown if needed
	UpdateScrollbarVisibility();

	int32 ColumnsToUse = (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns;
	int32 TargetRow = TargetSlotIndex / ColumnsToUse;

	// Get the available height of the scroll box
	float AvailableHeight = ItemScrollBox->GetCachedGeometry().GetLocalSize().Y;
	float RowHeight = SlotSize + (SlotPadding.Top + SlotPadding.Bottom);

	if (AvailableHeight <= 0.0f || RowHeight <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScrollToSlot: Invalid geometry (AvailableHeight=%.1f, RowHeight=%.1f)"), AvailableHeight, RowHeight);
		return;
	}

	// Calculate how many rows can fit in the visible area
	int32 VisibleRows = FMath::FloorToInt(AvailableHeight / RowHeight);

	// Get current scroll offset
	float CurrentScrollOffset = ItemScrollBox->GetScrollOffset();
	int32 FirstVisibleRow = FMath::FloorToInt(CurrentScrollOffset / RowHeight);
	int32 LastVisibleRow = FirstVisibleRow + VisibleRows - 1;

	UE_LOG(LogTemp, Warning, TEXT("ScrollToSlot: Slot %d is in row %d, currently visible rows %d-%d"),
		TargetSlotIndex, TargetRow, FirstVisibleRow, LastVisibleRow);

	// Check if target row is already visible
	if (TargetRow >= FirstVisibleRow && TargetRow <= LastVisibleRow)
	{
		UE_LOG(LogTemp, Log, TEXT("ScrollToSlot: Slot %d is already visible"), TargetSlotIndex);
		return;
	}

	// Calculate the scroll offset to make the target row visible
	float TargetScrollOffset;
	if (TargetRow < FirstVisibleRow)
	{
		// Target is above visible area - scroll to show it at the top
		TargetScrollOffset = TargetRow * RowHeight;
	}
	else
	{
		// Target is below visible area - scroll to show it at the bottom
		TargetScrollOffset = (TargetRow - VisibleRows + 1) * RowHeight;
		TargetScrollOffset = FMath::Max(0.0f, TargetScrollOffset);
	}

	ItemScrollBox->SetScrollOffset(TargetScrollOffset);
	UE_LOG(LogTemp, Warning, TEXT("ScrollToSlot: Scrolled to offset %.1f to show row %d"), TargetScrollOffset, TargetRow);
}

int32 UInventoryWidget::CalculateColumnsFromWidth(float AvailableWidth) const
{
	if (AvailableWidth <= 0.0f)
	{
		return GridColumns; // Fallback to default
	}

	// Account for padding on both sides of each slot
	float SlotTotalWidth = SlotSize + (SlotPadding.Left + SlotPadding.Right);

	// Calculate how many slots can fit
	int32 ColumnsFit = FMath::FloorToInt(AvailableWidth / SlotTotalWidth);

	// Clamp to reasonable values
	ColumnsFit = FMath::Clamp(ColumnsFit, 1, MaxPreCreatedColumns);

	return ColumnsFit;
}

void UInventoryWidget::ReflowItemsToGrid()
{
	if (!InventoryComponent || !ItemGrid || SlotWidgets.Num() == 0)
	{
		return;
	}

	int32 ColumnsToUse = (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns;

	UE_LOG(LogTemp, Warning, TEXT("ReflowItemsToGrid: Reflowing to %d columns"), ColumnsToUse);

	// Clear the grid
	ItemGrid->ClearChildren();

	// Re-add all slots to the grid with the new column layout
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		if (SlotWidgets[i])
		{
			int32 Row = i / ColumnsToUse;
			int32 Column = i % ColumnsToUse;

			// Get the SizeBox parent of the slot widget
			UWidget* SlotParent = SlotWidgets[i]->GetParent();
			if (SlotParent)
			{
				// Add the SizeBox back to the grid at the new position
				ItemGrid->AddChildToUniformGrid(SlotParent, Row, Column);
			}
		}
	}

	// Update clipping settings with the new layout
	ApplyClippingSettings();

	UE_LOG(LogTemp, Warning, TEXT("ReflowItemsToGrid: Grid reflowed with %d columns"), ColumnsToUse);
}

void UInventoryWidget::ExecuteDebouncedReflow()
{
	// Check if column count changed (horizontal resize)
	bool bColumnsChanged = (PendingColumnCount > 0 && PendingColumnCount != CurrentVisibleColumns);

	if (!bColumnsChanged)
	{
		// No column change, but we were called - this means vertical resize
		// Just compress items to fit the new visible area
		UE_LOG(LogTemp, Warning, TEXT("ExecuteDebouncedReflow: Vertical resize detected, compressing items"));
		CompressItems();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ExecuteDebouncedReflow: Reflowing from %d to %d columns after resize complete"),
		CurrentVisibleColumns, PendingColumnCount);

	CurrentVisibleColumns = PendingColumnCount;

	// First reflow the grid layout
	ReflowItemsToGrid();

	// Then compress items to move any out-of-bounds items back into view
	CompressItems();
}

void UInventoryWidget::OnGridViewButtonClicked()
{
	SetViewMode(false); // false = grid view
}

void UInventoryWidget::OnListViewButtonClicked()
{
	SetViewMode(true); // true = list view
}

void UInventoryWidget::OnColumnHeaderClicked(FName ColumnName)
{
	if (!InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("OnColumnHeaderClicked: Column '%s' clicked"), *ColumnName.ToString());

	// Determine sort direction based on current header state
	bool bAscending = true;
	if (ListViewHeader)
	{
		// If clicking the same column, toggle the direction
		if (ListViewHeader->CurrentSortColumn == ColumnName)
		{
			bAscending = !ListViewHeader->bSortAscending;
			UE_LOG(LogTemp, Log, TEXT("OnColumnHeaderClicked: Same column, toggling from %s to %s"),
				ListViewHeader->bSortAscending ? TEXT("Ascending") : TEXT("Descending"),
				bAscending ? TEXT("Ascending") : TEXT("Descending"));
		}
		else
		{
			// New column, start with ascending
			bAscending = true;
			UE_LOG(LogTemp, Log, TEXT("OnColumnHeaderClicked: New column, setting to Ascending"));
		}

		// Update the header to show the new sort state
		ListViewHeader->SetSortColumn(ColumnName, bAscending);
	}

	// Create a lambda predicate based on the selected column
	auto SortPredicate = [ColumnName, bAscending](const FInventoryItem& A, const FInventoryItem& B) -> bool
	{
		if (ColumnName == TEXT("Name"))
		{
			int32 Compare = A.ItemData->ItemName.ToString().Compare(B.ItemData->ItemName.ToString());
			return bAscending ? (Compare < 0) : (Compare > 0);
		}
		else if (ColumnName == TEXT("Quantity"))
		{
			return bAscending ? (A.Quantity < B.Quantity) : (A.Quantity > B.Quantity);
		}
		else if (ColumnName == TEXT("Weight"))
		{
			float WeightA = A.GetTotalWeight();
			float WeightB = B.GetTotalWeight();
			return bAscending ? (WeightA < WeightB) : (WeightA > WeightB);
		}
		else if (ColumnName == TEXT("Value"))
		{
			int32 ValueA = A.GetTotalValue();
			int32 ValueB = B.GetTotalValue();
			return bAscending ? (ValueA < ValueB) : (ValueA > ValueB);
		}
		else if (ColumnName == TEXT("Rarity"))
		{
			return bAscending ? (A.ItemData->Rarity < B.ItemData->Rarity) : (A.ItemData->Rarity > B.ItemData->Rarity);
		}
		else if (ColumnName == TEXT("Category"))
		{
			return bAscending ? (A.ItemData->Category < B.ItemData->Category) : (A.ItemData->Category > B.ItemData->Category);
		}

		return false;
	};

	// Use the InventoryComponent's custom sort method
	InventoryComponent->SortInventoryCustom(SortPredicate);

	// Refresh both grid and list views
	RefreshInventory();
}

void UInventoryWidget::ToggleViewMode()
{
	SetViewMode(!bIsListView);
}

void UInventoryWidget::SetViewMode(bool bListView)
{
	UE_LOG(LogTemp, Warning, TEXT("========== SetViewMode Called =========="));
	UE_LOG(LogTemp, Warning, TEXT("Switching to: %s"), bListView ? TEXT("LIST VIEW") : TEXT("GRID VIEW"));

	bIsListView = bListView;

	// Compress items before switching views to ensure proper layout
	CompressItems();

	// If using WidgetSwitcher (preferred method)
	if (ViewModeSwitcher)
	{
		int32 TargetIndex = bIsListView ? 1 : 0;
		ViewModeSwitcher->SetActiveWidgetIndex(TargetIndex);
		UE_LOG(LogTemp, Log, TEXT("ViewModeSwitcher: Set to index %d"), TargetIndex);

		// Populate list view when switching to it
		if (bIsListView)
		{
			// Use table view if available, otherwise fall back to list view
			if (ListViewRowContainer)
			{
				PopulateTableView();
				// Sync selections from grid to list after populating
				SyncSelectionsGridToList();
			}
			else if (ItemListView)
			{
				// Wait for multiple frames to ensure geometry is calculated
				if (UWorld* World = GetWorld())
				{
					// First frame delay - let switcher update
					FTimerHandle FirstDelayHandle;
					World->GetTimerManager().SetTimer(FirstDelayHandle, [this, World]()
					{
						if (ItemListView)
						{
							// Populate the list
							PopulateListView();

							// Second frame delay - force regenerate after list is populated
							FTimerHandle SecondDelayHandle;
							World->GetTimerManager().SetTimer(SecondDelayHandle, [this]()
							{
								if (ItemListView)
								{
									// Force the list to regenerate/refresh its visible entries
									ItemListView->RequestRefresh();
									ItemListView->RegenerateAllEntries();

									UE_LOG(LogTemp, Warning, TEXT("ListView regenerated. Final size: %.2f x %.2f"),
										ItemListView->GetCachedGeometry().GetLocalSize().X,
										ItemListView->GetCachedGeometry().GetLocalSize().Y);
								}
							}, 0.1f, false);
						}
					}, 0.05f, false);
				}
				else
				{
					PopulateListView();
				}
			}
		}
		else
		{
			// Refresh grid view when switching to it
			RefreshInventory();
			// Sync selections from list to grid after refreshing
			SyncSelectionsListToGrid();
		}
	}
	else
	{
		// Fallback to show/hide method
		UE_LOG(LogTemp, Warning, TEXT("ViewModeSwitcher not found, using visibility toggle method"));

		// Show/hide the appropriate view
		if (ItemGrid)
		{
			ItemGrid->SetVisibility(bIsListView ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			UE_LOG(LogTemp, Log, TEXT("ItemGrid visibility: %s"), bIsListView ? TEXT("Collapsed") : TEXT("Visible"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemGrid is NULL!"));
		}

		if (ItemScrollBox)
		{
			ItemScrollBox->SetVisibility(bIsListView ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			UE_LOG(LogTemp, Log, TEXT("ItemScrollBox visibility: %s"), bIsListView ? TEXT("Collapsed") : TEXT("Visible"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemScrollBox is NULL!"));
		}

		if (ItemListView)
		{
			UE_LOG(LogTemp, Log, TEXT("ItemListView found! Setting visibility..."));
			ItemListView->SetVisibility(bIsListView ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

			// Log ListView details
			UE_LOG(LogTemp, Log, TEXT("ItemListView Visibility: %s"), bIsListView ? TEXT("Visible") : TEXT("Collapsed"));
			UE_LOG(LogTemp, Log, TEXT("ItemListView Current Item Count: %d"), ItemListView->GetNumItems());
			UE_LOG(LogTemp, Log, TEXT("ItemListView IsVisible: %s"), ItemListView->GetVisibility() == ESlateVisibility::Visible ? TEXT("YES") : TEXT("NO"));

			// Populate list view when switching to it
			if (bIsListView)
			{
				UE_LOG(LogTemp, Warning, TEXT("About to populate list view..."));

				// Use table view if available, otherwise fall back to list view
				if (ListViewRowContainer)
				{
					// Delay population slightly to allow geometry to update
					if (UWorld* World = GetWorld())
					{
						FTimerHandle TempHandle;
						World->GetTimerManager().SetTimer(TempHandle, [this]()
						{
							PopulateTableView();
							// Sync selections from grid to list after populating
							SyncSelectionsGridToList();
						}, 0.1f, false);
					}
					else
					{
						PopulateTableView();
						// Sync selections from grid to list after populating
						SyncSelectionsGridToList();
					}
				}
				else if (ItemListView)
				{
					// Delay population slightly to allow geometry to update
					if (UWorld* World = GetWorld())
					{
						FTimerHandle TempHandle;
						World->GetTimerManager().SetTimer(TempHandle, [this]()
						{
							if (ItemListView)
							{
								PopulateListView();
							}
						}, 0.1f, false);
					}
					else
					{
						PopulateListView();
					}
				}
			}
			else
			{
				// Switching to grid view - sync selections from list to grid
				SyncSelectionsListToGrid();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemListView is NULL! Did you add a ListView widget named 'ItemListView' to WBP_Inventory?"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("========== SetViewMode Complete =========="));
}

void UInventoryWidget::PopulateListView()
{
	if (!ItemListView)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateListView: ItemListView is NULL!"));
		return;
	}

	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateListView: InventoryComponent is NULL!"));
		return;
	}

	// Check if Entry Widget Class is set
	UE_LOG(LogTemp, Warning, TEXT("PopulateListView: Checking ListView configuration..."));

	// Try to get the entry widget class from the ListView
	// Note: We can't directly access it, but we can check after adding items

	// Clear existing items
	ItemListView->ClearListItems();

	TArray<FInventoryItem> Items = InventoryComponent->GetAllItems();
	UE_LOG(LogTemp, Log, TEXT("PopulateListView: Total items in inventory: %d"), Items.Num());

	int32 ValidItemCount = 0;
	int32 FilteredOutCount = 0;

	// Add items to list view
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		const FInventoryItem& Item = Items[i];

		// Only add valid items (skip empty slots)
		if (Item.IsValid())
		{
			ValidItemCount++;

			// Check filter
			if (!PassesFilter(Item))
			{
				FilteredOutCount++;
				UE_LOG(LogTemp, Verbose, TEXT("PopulateListView: Item at slot %d filtered out"), i);
				continue;
			}

			// Create data object for this item
			UInventoryListItemData* ItemData = NewObject<UInventoryListItemData>(this);
			ItemData->Item = Item;
			ItemData->SlotIndex = i;
			ItemData->InventoryComponent = InventoryComponent;

			// Add to list view
			ItemListView->AddItem(ItemData);

			UE_LOG(LogTemp, Log, TEXT("PopulateListView: Added item '%s' at slot %d to list view"),
				*Item.ItemData->ItemName.ToString(), i);
		}
	}

	// Set column settings on all visible row widgets
	if (ColumnSettings)
	{
		TArray<UUserWidget*> DisplayedEntries = ItemListView->GetDisplayedEntryWidgets();
		for (UUserWidget* EntryWidget : DisplayedEntries)
		{
			if (UInventoryListRowWidget* RowWidget = Cast<UInventoryListRowWidget>(EntryWidget))
			{
				RowWidget->SetColumnSettings(ColumnSettings);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PopulateListView: Valid items: %d, Filtered out: %d, Added to list: %d"),
		ValidItemCount, FilteredOutCount, ItemListView->GetNumItems());

	// Check ListView configuration
	FVector2D ListViewSize = ItemListView->GetCachedGeometry().GetLocalSize();
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
	UE_LOG(LogTemp, Warning, TEXT("ListView Diagnostics:"));
	UE_LOG(LogTemp, Warning, TEXT("  Size: %.2f x %.2f"), ListViewSize.X, ListViewSize.Y);
	UE_LOG(LogTemp, Warning, TEXT("  Items in list: %d"), ItemListView->GetNumItems());
	UE_LOG(LogTemp, Warning, TEXT("  Visibility: %d"), (int32)ItemListView->GetVisibility());

	if (ListViewSize.X <= 0 || ListViewSize.Y <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT(""));
		UE_LOG(LogTemp, Error, TEXT("PROBLEM: ListView has ZERO SIZE!"));
		UE_LOG(LogTemp, Error, TEXT("Fix in Blueprint:"));
		UE_LOG(LogTemp, Error, TEXT("  1. Open WBP_Inventory"));
		UE_LOG(LogTemp, Error, TEXT("  2. Select ItemListView widget"));
		UE_LOG(LogTemp, Error, TEXT("  3. Set Size to 'Fill' or give it explicit dimensions"));
		UE_LOG(LogTemp, Error, TEXT("  4. Make sure it's not inside a collapsed container"));
		UE_LOG(LogTemp, Error, TEXT(""));
	}

	UE_LOG(LogTemp, Warning, TEXT("========================================"));

	// Force refresh the list view
	ItemListView->RequestRefresh();
}

void UInventoryWidget::PopulateTableView()
{
	if (!ListViewRowContainer || !ListRowWidgetClass || !InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateTableView: Missing required components!"));
		UE_LOG(LogTemp, Warning, TEXT("  ListViewRowContainer: %s"), ListViewRowContainer ? TEXT("OK") : TEXT("NULL"));
		UE_LOG(LogTemp, Warning, TEXT("  ListRowWidgetClass: %s"), ListRowWidgetClass ? TEXT("OK") : TEXT("NULL"));
		UE_LOG(LogTemp, Warning, TEXT("  InventoryComponent: %s"), InventoryComponent ? TEXT("OK") : TEXT("NULL"));
		return;
	}

	// Clear existing rows
	ListViewRowContainer->ClearChildren();
	TableViewRows.Empty();

	TArray<FInventoryItem> Items = InventoryComponent->GetAllItems();
	UE_LOG(LogTemp, Log, TEXT("PopulateTableView: Total items in inventory: %d"), Items.Num());

	int32 ValidItemCount = 0;
	int32 FilteredOutCount = 0;

	// Create row widgets for each valid item
	int32 VisualRowIndex = 0; // Track visual row order (different from slot index)
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		const FInventoryItem& Item = Items[i];

		// Only add valid items (skip empty slots)
		if (Item.IsValid())
		{
			ValidItemCount++;

			// Check filter
			if (!PassesFilter(Item))
			{
				FilteredOutCount++;
				continue;
			}

			// Create row widget
			UInventoryListRowWidget* RowWidget = CreateWidget<UInventoryListRowWidget>(this, ListRowWidgetClass);
			if (RowWidget)
			{
				// Create data object for this item
				UInventoryListItemData* ItemData = NewObject<UInventoryListItemData>(this);
				ItemData->Item = Item;
				ItemData->SlotIndex = i;
				ItemData->InventoryComponent = InventoryComponent;

				// Set data on row widget
				RowWidget->ItemData = ItemData;
				RowWidget->SetColumnSettings(ColumnSettings);
				RowWidget->SetRowIndex(VisualRowIndex); // Set row index for alternating colors
				RowWidget->RefreshDisplay();

				// Add to vertical box
				UVerticalBoxSlot* RowSlot = ListViewRowContainer->AddChildToVerticalBox(RowWidget);
				if (RowSlot)
				{
					RowSlot->SetPadding(FMargin(0.0f));
					RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				}

				// Cache the row widget
				TableViewRows.Add(RowWidget);

				UE_LOG(LogTemp, Log, TEXT("PopulateTableView: Added item '%s' at slot %d (visual row %d)"),
					*Item.ItemData->ItemName.ToString(), i, VisualRowIndex);

				VisualRowIndex++; // Increment visual row counter
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PopulateTableView: Valid items: %d, Filtered out: %d, Rows created: %d"),
		ValidItemCount, FilteredOutCount, TableViewRows.Num());
}

void UInventoryWidget::SyncSelectionsGridToList()
{
	if (!InventoryComponent || SlotWidgets.IsEmpty())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("SyncSelectionsGridToList: Syncing selections from grid to list view"));

	// Get selected slots from grid view
	TArray<int32> SelectedSlotIndices = SlotWidgets[0]->GetSelectedSlots();

	if (SelectedSlotIndices.IsEmpty())
	{
		// No selections in grid, clear all list selections
		UInventoryListRowWidget::ClearAllRowSelections();
		UE_LOG(LogTemp, Log, TEXT("SyncSelectionsGridToList: No grid selections, cleared list selections"));
		return;
	}

	// Clear existing list selections first
	UInventoryListRowWidget::ClearAllRowSelections();

	// Convert slot indices to a set for fast lookup
	TSet<int32> SelectedSlotSet(SelectedSlotIndices);

	// Select corresponding rows in list view
	int32 SelectionCount = 0;
	for (UInventoryListRowWidget* RowWidget : TableViewRows)
	{
		if (RowWidget && RowWidget->ItemData && SelectedSlotSet.Contains(RowWidget->ItemData->SlotIndex))
		{
			RowWidget->SelectRow(true); // Add to selection
			SelectionCount++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("SyncSelectionsGridToList: Synchronized %d selections to list view"), SelectionCount);
}

void UInventoryWidget::SyncSelectionsListToGrid()
{
	if (!InventoryComponent || SlotWidgets.IsEmpty())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("SyncSelectionsListToGrid: Syncing selections from list to grid view"));

	// Get selected rows from list view
	const TSet<UInventoryListRowWidget*>& SelectedRows = UInventoryListRowWidget::GetSelectedRows();

	if (SelectedRows.IsEmpty())
	{
		// No selections in list, clear all grid selections
		UInventorySlotWidget::ClearAllSelectionsForInventory(InventoryComponent);
		UE_LOG(LogTemp, Log, TEXT("SyncSelectionsListToGrid: No list selections, cleared grid selections"));
		return;
	}

	// Clear existing grid selections first
	UInventorySlotWidget::ClearAllSelectionsForInventory(InventoryComponent);

	// Collect slot indices to select
	TArray<int32> SlotIndicesToSelect;
	for (UInventoryListRowWidget* RowWidget : SelectedRows)
	{
		if (RowWidget && RowWidget->ItemData)
		{
			SlotIndicesToSelect.Add(RowWidget->ItemData->SlotIndex);
		}
	}

	// Select corresponding slots in grid view
	int32 SelectionCount = 0;
	for (int32 SlotIndex : SlotIndicesToSelect)
	{
		if (SlotWidgets.IsValidIndex(SlotIndex) && SlotWidgets[SlotIndex])
		{
			SlotWidgets[SlotIndex]->SetSelected(true);
			SelectionCount++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("SyncSelectionsListToGrid: Synchronized %d selections to grid view"), SelectionCount);
}

void UInventoryWidget::HandleContextMenuAction_Implementation(FName ActionID)
{
	UE_LOG(LogTemp, Warning, TEXT("===== INVENTORY WIDGET HandleContextMenuAction ====="));
	UE_LOG(LogTemp, Warning, TEXT("ActionID: %s"), *ActionID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent valid: %s"), InventoryComponent ? TEXT("YES") : TEXT("NO"));

	if (ActionID == "StackAll" || ActionID == "StackAllEmpty")
	{
		HandleStackAllEmpty();
	}
	else if (ActionID == "SelectAll")
	{
		HandleSelectAll();
	}
	else if (ActionID == "InvertSelection")
	{
		HandleInvertSelection();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UNKNOWN ACTION ID: '%s'"), *ActionID.ToString());
	}
}

void UInventoryWidget::HandleStackAllEmpty()
{
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("HandleStackAllEmpty: InventoryComponent is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("========== Stack All (Empty) triggered - consolidating all stackable items =========="));

	// Get all items
	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	UE_LOG(LogTemp, Warning, TEXT("HandleStackAllEmpty: Found %d total items"), AllItems.Num());

	// Group items by ItemID to find stackable groups
	TMap<FName, TArray<int32>> ItemGroups;
	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		if (AllItems[i].IsValid() && AllItems[i].ItemData && AllItems[i].ItemData->MaxStackSize > 1)
		{
			FName ItemID = AllItems[i].ItemData->ItemID;
			ItemGroups.FindOrAdd(ItemID).Add(i);
			UE_LOG(LogTemp, Log, TEXT("  Slot %d: %s (qty=%d, max=%d)"),
				i, *AllItems[i].ItemData->ItemName.ToString(), AllItems[i].Quantity, AllItems[i].ItemData->MaxStackSize);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("HandleStackAllEmpty: Found %d different item types to potentially stack"), ItemGroups.Num());

	// For each group, try to stack them together
	for (const auto& Group : ItemGroups)
	{
		const TArray<int32>& SlotIndices = Group.Value;
		if (SlotIndices.Num() > 1)
		{
			// Sort slots so we process them in order
			TArray<int32> SortedSlots = SlotIndices;
			SortedSlots.Sort();

			// Merge all stacks together - process from the end backwards
			for (int32 i = SortedSlots.Num() - 1; i > 0; i--)
			{
				int32 SourceSlot = SortedSlots[i];

				// Try to find a target slot that has space (search from beginning)
				for (int32 j = 0; j < i; j++)
				{
					int32 TargetSlot = SortedSlots[j];

					// Re-fetch items each time to get current state
					FInventoryItem SourceItem = InventoryComponent->GetItemAtSlot(SourceSlot);
					FInventoryItem TargetItem = InventoryComponent->GetItemAtSlot(TargetSlot);

					// Skip if source is already empty (already merged)
					if (!SourceItem.IsValid())
					{
						break;
					}

					// Check if target has space
					if (TargetItem.IsValid() && TargetItem.Quantity < TargetItem.ItemData->MaxStackSize)
					{
						UE_LOG(LogTemp, Log, TEXT("  Merging slot %d (qty=%d) into slot %d (qty=%d)"),
							SourceSlot, SourceItem.Quantity, TargetSlot, TargetItem.Quantity);

						InventoryComponent->MergeStacks(SourceSlot, TargetSlot);

						// Check if source is now empty
						SourceItem = InventoryComponent->GetItemAtSlot(SourceSlot);
						if (!SourceItem.IsValid())
						{
							break;
						}
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Stack All completed"));

	// Refresh the display after stacking
	if (bIsListView)
	{
		if (ListViewRowContainer)
		{
			PopulateTableView();
		}
		else if (ItemListView)
		{
			PopulateListView();
		}
	}
	else
	{
		RefreshInventory();
	}
}

void UInventoryWidget::HandleSelectAll()
{
	UE_LOG(LogTemp, Log, TEXT("Select All triggered"));

	if (bIsListView)
	{
		// Select all rows in list view
		for (UInventoryListRowWidget* Row : TableViewRows)
		{
			if (Row && Row->IsValidLowLevel())
			{
				Row->SelectRow(true);
			}
		}
	}
	else
	{
		// Select all slots in grid view
		for (UInventorySlotWidget* SlotWidget : SlotWidgets)
		{
			if (SlotWidget && SlotWidget->IsValidLowLevel())
			{
				SlotWidget->SetSelected(true);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Select All completed"));
}

void UInventoryWidget::HandleInvertSelection()
{
	UE_LOG(LogTemp, Log, TEXT("Invert Selection triggered"));

	if (bIsListView)
	{
		// Invert selection of all rows in list view
		for (UInventoryListRowWidget* Row : TableViewRows)
		{
			if (Row && Row->IsValidLowLevel())
			{
				Row->ToggleSelection(true);
			}
		}
	}
	else
	{
		// Invert selection of all slots in grid view
		for (UInventorySlotWidget* SlotWidget : SlotWidgets)
		{
			if (SlotWidget && SlotWidget->IsValidLowLevel())
			{
				SlotWidget->ToggleSelection();
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Invert Selection completed"));
}
