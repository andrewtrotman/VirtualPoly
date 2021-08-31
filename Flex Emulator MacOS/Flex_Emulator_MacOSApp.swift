/*
	FLEX_EMULATOR_MACOSAPP.SWIFT
	----------------------------
*/
import SwiftUI

func clipboard_has_pastable_text() -> Bool
	{
//	NSPasteboard.general.string(forType:NSPasteboard.PasteboardType.string)

//	let answer = NSPasteboard.general.canReadItem(withDataConformingToTypes: <#T##[String]#>)

	let answer = NSPasteboard.general.pasteboardItems?.first?.string(forType: .string)
	return answer == nil
	}

@main
struct Flex_Emulator_MacOSApp: App
	{
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @StateObject var app_state = AppState.shared
	@State var zoooom = 1.0
	@State var zoom = 1.0
	@State var clipboard_has_text = clipboard_has_pastable_text()
	var body: some Scene
		{
		WindowGroup
			{
			ContentView(app_state: app_state)
			}
			.commands
				{
				CommandGroup(replacing: .pasteboard)
					{
					Button("Cut"){}.keyboardShortcut("X").disabled(true)
					Button("Copy"){}.keyboardShortcut("C")
					Button("Paste")
						{
						print("Pasting something...")

						if let myString = NSPasteboard.general.pasteboardItems?.first?.string(forType: .string)
							{
							print(myString)
							}
						}
						.keyboardShortcut("V")
					Button("Paste and Match Style") { print("Pasting and Matching something...") }
						.keyboardShortcut("V", modifiers: [.command, .option, .shift])
					Button("Delete"){}.keyboardShortcut(.delete)
					Button("Select All"){}.keyboardShortcut("A")
					}
				CommandGroup(replacing: .systemServices) {}
				CommandGroup(replacing: .newItem) {}
				CommandGroup(after: .windowSize)
					{
					Picker(selection: $zoooom, label: Text("Zoooom"))
						{
						Text("0.5x").tag(0.5)
						Text("1.0x").tag(1.0)
						Text("1.5x").tag(1.5)
						Text("2.0x").tag(2.0)
						}
						.onReceive([self.zoooom].publisher.first())
							{ value in
							print("Z")
							print(value)
							if app_state.screen_size != CGFloat(value)
								{
								app_state.screen_size = CGFloat(value)
								}
							}
					}
				CommandMenu("FLEX")
					{
					Picker(selection: $zoom, label: Text("Zoom"))
						{
						Text("0.5x").tag(0.5)
						Text("1.0x").tag(1.0)
						Text("1.5x").tag(1.5)
						Text("2.0x").tag(2.0)
						}
						.onReceive([self.zoom].publisher.first())
							{ value in
							print("X")
							print(value)
							if app_state.screen_size != CGFloat(value)
								{
								app_state.screen_size = CGFloat(value)
								}
							}
					}
				}
		}
	}

class AppDelegate: NSObject, NSApplicationDelegate
	{
    func applicationWillTerminate(_ aNotification: Notification)
		{
		print("Bye")
//		machine_serialise(AppState.shared.machine)
		}
	}

class AppState: ObservableObject
	{
	static let shared = AppState()

	@Published var screen_size: CGFloat = 1.0
	var machine: UnsafeRawPointer? = nil
	}
