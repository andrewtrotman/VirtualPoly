/*
	WEBVIEW.SWIFT
	-------------
*/
import SwiftUI
import WebKit

/*
	WEBVIEW
	-------
*/
struct WebView: NSViewRepresentable
	{
	@State var text: String

	/*
		MAKECOORDINATOR()
		-----------------
	*/
	func makeCoordinator() -> WebView.Coordinator
		{
		Coordinator(self)
		}

	/*
		MAKENSVIEW()
		------------
	*/
	func makeNSView(context: Context) -> WKWebView
		{
		let view = WKWebView()
        view.navigationDelegate = context.coordinator
		return view
		}
	/*
		UPDATENSVIEW()
		--------------
	*/
	func updateNSView(_ uiView: WKWebView, context: Context)
		{
		uiView.loadHTMLString(text, baseURL: nil)
		}

	/*
		CLASS COORDINATOR
		-----------------
	*/
	class Coordinator: NSObject, WKNavigationDelegate
		{
		let parent: WebView

		init(_ parent: WebView)
			{
			self.parent = parent
			}

		/*
			WEBVIEW()
			---------
		*/
		func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void)
			{
			if let url = navigationAction.request.url
				{
				if url.absoluteString.hasPrefix("http://") || url.absoluteString.hasPrefix("https://")
					{
					decisionHandler(.cancel)
					NSWorkspace.shared.open(url)
					return
					}
				}
			decisionHandler(.allow)
			}
		}
	}
