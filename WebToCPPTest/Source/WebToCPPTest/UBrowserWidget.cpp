#include "UBrowserWidget.h"

void UBrowserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!WebBrowser)
	{
		UE_LOG(LogTemp, Error, TEXT("WebBrowser not bound!"));
		return;
	}

	WebBrowser->ExecuteJavascript(TEXT("console.log('Hello from C++!');"));

	WebBrowser->LoadURL(GetLocalHtmlPath());

	// Use the URL as a hack to communicate JS-to-C++, as the web browser API available is extremely limited
	// otherwise.
	WebBrowser->OnUrlChanged.AddDynamic(this, &UBrowserWidget::HandleUrlChanged);
}

void UBrowserWidget::HandleUrlChanged(const FText& NewUrl)
{
	if (WebBrowser)
	{
		FString UrlString = NewUrl.ToString();
		UE_LOG(LogTemp, Display, TEXT("Data received from browser: %s"), *UrlString);
	}
}

FString UBrowserWidget::GetLocalHtmlPath()
{
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Web/index.html"));
	return FString::Printf(TEXT("file:///%s"), *AbsolutePath.Replace(TEXT("\\"), TEXT("/")));
}
