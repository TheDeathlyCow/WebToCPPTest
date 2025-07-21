#include "UBrowserWidget.h"
#include "AssetRegistryModule.h"
#include "IAssetRegistry.h"

void UBrowserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!WebBrowser)
	{
		UE_LOG(LogTemp, Error, TEXT("WebBrowser not bound!"));
		return;
	}

	WebBrowser->LoadURL(GetIndexHtmlPath());

	// I use this hack to communicate from JS to C++, as the web browser API available is extremely limited otherwise.
	// Essentially, it works by having the JS side update the query params of the window, which changes the URL without
	// changing the page, and allows JS to pass arbitrary data that C++ can pick up. 
	WebBrowser->OnUrlChanged.AddDynamic(this, &UBrowserWidget::HandleUrlChanged);
}

void UBrowserWidget::HandleUrlChanged(const FText& NewUrl)
{
	if (WebBrowser)
	{
		FString UrlString = NewUrl.ToString();
		UE_LOG(LogTemp, Display, TEXT("Data received from browser: %s"), *UrlString);

		ReloadAssetData();
	}
}

FString UBrowserWidget::GetIndexHtmlPath()
{
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Web/index.html"));
	return FString::Printf(TEXT("file:///%s"), *AbsolutePath.Replace(TEXT("\\"), TEXT("/")));
}

// this is called whenever the url is changed, including on the first load
void UBrowserWidget::ReloadAssetData() const
{
	TArray<FString> AssetList = GetAssetNames();

	FString JsonArray = TEXT("[");
	for (int32 i = 0; i < AssetList.Num(); ++i)
	{
		JsonArray += FString::Printf(TEXT("\"%s\""), *AssetList[i].ReplaceCharWithEscapedChar());
		if (i < AssetList.Num() - 1)
		{
			JsonArray += TEXT(",");
		}
	}
	JsonArray += TEXT("]");

	FString JSCommand= FString::Printf(TEXT("insertAssets(%s);"), *JsonArray);
	WebBrowser->ExecuteJavascript(JSCommand);

	FString& LogMessage = JSCommand;
	if (AssetList.Num() > 10)
	{
		LogMessage = FString::Printf(TEXT("insertAssets([/* an array with %d items */]);"), AssetList.Num()); 
	}

	UE_LOG(LogTemp, Display, TEXT("Data script executed in browser: %s"), *LogMessage);
}

TArray<FString> UBrowserWidget::GetAssetNames()
{
	TArray<FString> AssetNames;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.PackagePaths.Add("/");
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		AssetNames.Add(AssetData.AssetName.ToString());
	}

	return AssetNames;
}
