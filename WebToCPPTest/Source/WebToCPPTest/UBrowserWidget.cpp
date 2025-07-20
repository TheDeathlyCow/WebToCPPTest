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

	WebBrowser->OnUrlChanged.AddDynamic(this, &UBrowserWidget::HandleUrlChanged);
}

void UBrowserWidget::HandleUrlChanged(const FText& ButtonText)
{
	if (WebBrowser)
	{
		FString JS = TEXT("console.log('C++ saw you changed url :o');");
		WebBrowser->ExecuteJavascript(JS);
	}
}

FString UBrowserWidget::GetLocalHtmlPath() const
{
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Web/index.html"));
	return FString::Printf(TEXT("file:///%s"), *AbsolutePath.Replace(TEXT("\\"), TEXT("/")));
}
