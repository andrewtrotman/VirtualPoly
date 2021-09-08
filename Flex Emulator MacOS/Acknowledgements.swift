/*
	ACKNOWLEDGEMENTS.SWIFT
	----------------------
*/
import Foundation
import SwiftUI
import WebKit

/*
	MESSAGE()
	---------
*/
private func message() -> String
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

/*
	STRUCT ACKNOWLEDGEMENTS
	-----------------------
*/
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
		.onDisappear()
			{
			state = false
			}
		}
}
