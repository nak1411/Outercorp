// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/PanelSlot.h"
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
					if (InventoryWidget->SlotWidgets.Num() > 0 && InventoryWidget->SlotWidgets[0])
					{
						InventoryWidget->SlotWidgets[0]->ClearAllSelections();
					}
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
}

void UInventoryWidget::NativeDestruct()
{
	// Clean up timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SetupTimerHandle);
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
	// If clicking in the inventory widget (but not on a slot), deselect all items
	// The slots will handle their own clicks and prevent this from being called
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && InventoryComponent)
	{
		// Clear all selections
		if (SlotWidgets.Num() > 0 && SlotWidgets[0])
		{
			SlotWidgets[0]->ClearAllSelections();
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

	// Refresh all slots
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		RefreshSlot(i);
	}

	// Update capacity display
	UpdateCapacityDisplay();
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
		ItemScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
		ItemScrollBox->SetAllowOverscroll(false);
		ItemScrollBox->SetOrientation(EOrientation::Orient_Vertical);

		UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: Vertical ScrollBox configured"));
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

			// Let height auto-grow based on content
			ParentSizeBox->SetHeightOverride(0.0f);
			ParentSizeBox->SetMinDesiredHeight(0.0f);

			UE_LOG(LogTemp, Log, TEXT("ApplyClippingSettings: SizeBox - Fixed width: %f (%d columns), Auto height"), GridWidth, ColumnsForWidth);
		}
		else if (!GridSizeBox)
		{
			// Create a SizeBox wrapper at runtime to enforce fixed width
			GridSizeBox = NewObject<USizeBox>(this);
			GridSizeBox->SetWidthOverride(GridWidth);
			GridSizeBox->SetMinDesiredWidth(GridWidth);
			GridSizeBox->SetMaxDesiredWidth(GridWidth);
			GridSizeBox->SetHeightOverride(0.0f); // Auto height

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

	UE_LOG(LogTemp, Log, TEXT("CreateSlotWidgets: Grid created with %d columns, slots will not compress on resize"), ColumnsToUse);

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
