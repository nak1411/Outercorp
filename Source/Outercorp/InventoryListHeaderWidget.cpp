// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryListHeaderWidget.h"
#include "InventoryColumnSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"

void UInventoryListHeaderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind header button click events
	if (IconHeaderButton)
	{
		IconHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnIconHeaderClicked);
	}

	if (NameHeaderButton)
	{
		NameHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnNameHeaderClicked);
	}

	if (QuantityHeaderButton)
	{
		QuantityHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnQuantityHeaderClicked);
	}

	if (WeightHeaderButton)
	{
		WeightHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnWeightHeaderClicked);
	}

	if (ValueHeaderButton)
	{
		ValueHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnValueHeaderClicked);
	}

	if (RarityHeaderButton)
	{
		RarityHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnRarityHeaderClicked);
	}

	if (CategoryHeaderButton)
	{
		CategoryHeaderButton->OnClicked.AddDynamic(this, &UInventoryListHeaderWidget::OnCategoryHeaderClicked);
	}

	// Set background color for header
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	// Initialize sort indicators
	UpdateSortIndicators();
}

void UInventoryListHeaderWidget::SetSortColumn(FName ColumnName, bool bAscending)
{
	CurrentSortColumn = ColumnName;
	bSortAscending = bAscending;
	UpdateSortIndicators();
}

void UInventoryListHeaderWidget::OnIconHeaderClicked()
{
	// Icon column doesn't support sorting
}

void UInventoryListHeaderWidget::OnNameHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Name"));
}

void UInventoryListHeaderWidget::OnQuantityHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Quantity"));
}

void UInventoryListHeaderWidget::OnWeightHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Weight"));
}

void UInventoryListHeaderWidget::OnValueHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Value"));
}

void UInventoryListHeaderWidget::OnRarityHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Rarity"));
}

void UInventoryListHeaderWidget::OnCategoryHeaderClicked()
{
	// Just broadcast the event - let the InventoryWidget handle the sorting logic
	OnColumnHeaderClicked.Broadcast(TEXT("Category"));
}

void UInventoryListHeaderWidget::UpdateSortIndicators()
{
	FString SortIndicator = bSortAscending ? TEXT("▲") : TEXT("▼");

	// Update all header texts - keep base text static
	if (NameHeaderText)
	{
		NameHeaderText->SetText(FText::FromString(TEXT("Name")));
	}

	if (QuantityHeaderText)
	{
		QuantityHeaderText->SetText(FText::FromString(TEXT("Qty")));
	}

	if (WeightHeaderText)
	{
		WeightHeaderText->SetText(FText::FromString(TEXT("Weight")));
	}

	if (ValueHeaderText)
	{
		ValueHeaderText->SetText(FText::FromString(TEXT("Value")));
	}

	if (RarityHeaderText)
	{
		RarityHeaderText->SetText(FText::FromString(TEXT("Rarity")));
	}

	if (CategoryHeaderText)
	{
		CategoryHeaderText->SetText(FText::FromString(TEXT("Category")));
	}

	// Update separate arrow text blocks - show only on active column
	if (NameSortArrow)
	{
		NameSortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Name") ? SortIndicator : TEXT("")));
		NameSortArrow->SetVisibility(CurrentSortColumn == TEXT("Name") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (QuantitySortArrow)
	{
		QuantitySortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Quantity") ? SortIndicator : TEXT("")));
		QuantitySortArrow->SetVisibility(CurrentSortColumn == TEXT("Quantity") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (WeightSortArrow)
	{
		WeightSortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Weight") ? SortIndicator : TEXT("")));
		WeightSortArrow->SetVisibility(CurrentSortColumn == TEXT("Weight") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (ValueSortArrow)
	{
		ValueSortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Value") ? SortIndicator : TEXT("")));
		ValueSortArrow->SetVisibility(CurrentSortColumn == TEXT("Value") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (RaritySortArrow)
	{
		RaritySortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Rarity") ? SortIndicator : TEXT("")));
		RaritySortArrow->SetVisibility(CurrentSortColumn == TEXT("Rarity") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (CategorySortArrow)
	{
		CategorySortArrow->SetText(FText::FromString(CurrentSortColumn == TEXT("Category") ? SortIndicator : TEXT("")));
		CategorySortArrow->SetVisibility(CurrentSortColumn == TEXT("Category") ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UInventoryListHeaderWidget::SetColumnSettings(UInventoryColumnSettings* InColumnSettings)
{
	ColumnSettings = InColumnSettings;

	if (ColumnSettings)
	{
		// Bind to column width changes
		ColumnSettings->OnColumnWidthsChanged.AddDynamic(this, &UInventoryListHeaderWidget::OnColumnWidthsChanged);

		// Initial update
		UpdateColumnWidths();
	}
}

void UInventoryListHeaderWidget::UpdateColumnWidths()
{
	if (!ColumnSettings)
	{
		return;
	}

	// Update SizeBox widths for each column
	if (IconColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Icon"));
		IconColumnBox->SetWidthOverride(Width);
		IconColumnBox->SetMinDesiredWidth(Width);
		IconColumnBox->SetMaxDesiredWidth(Width);
	}

	if (NameColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Name"));
		NameColumnBox->SetWidthOverride(Width);
		NameColumnBox->SetMinDesiredWidth(Width);
		NameColumnBox->SetMaxDesiredWidth(Width);
	}

	if (QuantityColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Quantity"));
		QuantityColumnBox->SetWidthOverride(Width);
		QuantityColumnBox->SetMinDesiredWidth(Width);
		QuantityColumnBox->SetMaxDesiredWidth(Width);
	}

	if (WeightColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Weight"));
		WeightColumnBox->SetWidthOverride(Width);
		WeightColumnBox->SetMinDesiredWidth(Width);
		WeightColumnBox->SetMaxDesiredWidth(Width);
	}

	if (ValueColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Value"));
		ValueColumnBox->SetWidthOverride(Width);
		ValueColumnBox->SetMinDesiredWidth(Width);
		ValueColumnBox->SetMaxDesiredWidth(Width);
	}

	if (RarityColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Rarity"));
		RarityColumnBox->SetWidthOverride(Width);
		RarityColumnBox->SetMinDesiredWidth(Width);
		RarityColumnBox->SetMaxDesiredWidth(Width);
	}

	if (CategoryColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Category"));
		CategoryColumnBox->SetWidthOverride(Width);
		CategoryColumnBox->SetMinDesiredWidth(Width);
		CategoryColumnBox->SetMaxDesiredWidth(Width);
	}
}

void UInventoryListHeaderWidget::OnColumnWidthsChanged()
{
	UpdateColumnWidths();
}
