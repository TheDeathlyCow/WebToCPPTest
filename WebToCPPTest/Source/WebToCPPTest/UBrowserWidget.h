#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "UBrowserWidget.generated.h"

UCLASS(Blueprintable)
class UBrowserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UWebBrowser* WebBrowser;

private:
	UFUNCTION()
	void HandleUrlChanged(const FText& NewUrl);

	UFUNCTION()
	static FString GetIndexHtmlPath();

	UFUNCTION()
	void ReloadAssetData() const;

	UFUNCTION()
	static TArray<FString> GetAssetNames();

	UFUNCTION()
	static bool GetQueryParamsFromUrl(const FString& Url, TMap<FString, FString>& QueryParams);
};
