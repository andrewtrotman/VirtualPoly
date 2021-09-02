//
//  Acknowledgements.swift
//  Flex Emulator MacOS
//
//  Created by Andrew Trotman on 1/09/21.
//

import Foundation
import SwiftUI


import WebKit



struct WebView: NSViewRepresentable
	{
	@State var text: String

    func makeCoordinator() -> WebView.Coordinator
    	{
        Coordinator(self)
        }

	func makeNSView(context: Context) -> WKWebView
		{
		let view = WKWebView()
        view.navigationDelegate = context.coordinator
		return view
		}

	func updateNSView(_ uiView: WKWebView, context: Context)
		{
		uiView.loadHTMLString(text, baseURL: nil)
		}

	class Coordinator: NSObject, WKNavigationDelegate
		{
		let parent: WebView

		init(_ parent: WebView)
			{
			self.parent = parent
			}

		func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void)
			{
			if let url = navigationAction.request.url
				{
				if url.absoluteString.hasPrefix("http://") || url.absoluteString.hasPrefix("https://")
					{
					decisionHandler(.cancel)
					NSWorkspace.shared.open(url)
					}
				else
					{
					decisionHandler(.allow)
					}
				}
			else
				{
				decisionHandler(.allow)
				}
			}
		}
	}

func message() -> String
	{
	do
		{
		let answer = try String(contentsOfFile: Bundle.main.path(forResource: "Acknowledgements", ofType: "html")!)
		return answer
		}
	catch
		{
		return ""
		}
}

struct Acknowledgements: View
	{
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
    @Binding var state: Bool

	var thanks = message()

	var body: some View
		{
		VStack
			{
			Spacer()
			WebView(text: thanks)
                .padding()
			Spacer()
			Button("OK")
				{
				state = false
				NSApplication.shared.keyWindow?.close()
				}
			Spacer()
			}
		.frame(width:(NSScreen.main?.visibleFrame.size.width)! / 2.0, height:(NSScreen.main?.visibleFrame.size.height)! / 2.0)
		}
	}



