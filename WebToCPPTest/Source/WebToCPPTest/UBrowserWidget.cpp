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
	if (!WebBrowser)
	{
		UE_LOG(LogTemp, Error, TEXT("Web browser not bound"));
		return;
	}

	FString UrlString = NewUrl.ToString();
	ReloadAssetData();

	TMap<FString, FString> QueryParams;
	if (GetQueryParamsFromUrl(UrlString, QueryParams))
	{
		FString AssetName = QueryParams[TEXT("assetName")];
		UE_LOG(LogTemp, Display, TEXT("Selected Asset: %s"), *AssetName);

		FString JSCommand = FString::Printf(TEXT("setSelectedAsset('%s');"), *AssetName);
		WebBrowser->ExecuteJavascript(JSCommand);
		UE_LOG(LogTemp, Display, TEXT("Data script executed in browser: %s"), *JSCommand);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to parse query params"));
	}

	UE_LOG(LogTemp, Display, TEXT("Data received from browser: %s"), *UrlString);
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

	FString JSCommand = FString::Printf(TEXT("insertAssets(%s);"), *JsonArray);
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

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
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

bool UBrowserWidget::GetQueryParamsFromUrl(const FString& Url, TMap<FString, FString>& QueryParams)
{
	int32 QueryIndex;
	if (Url.FindChar('?', QueryIndex))
	{
		FString QueryString = Url.Mid(QueryIndex + 1);

		TArray<FString> Pairs;
		QueryString.ParseIntoArray(Pairs, TEXT("&"), true);

		for (const FString& Pair : Pairs)
		{
			FString Key, Value;
			if (Pair.Split(TEXT("="), &Key, &Value))
			{
				QueryParams.Add(Key, Value);
			}
		}

		return true;
	}

	return false;
}
