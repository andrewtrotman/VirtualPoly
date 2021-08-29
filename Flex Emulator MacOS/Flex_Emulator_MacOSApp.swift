/*
	FLEX_EMULATOR_MACOSAPP.SWIFT
	----------------------------
*/
import SwiftUI

@main
struct Flex_Emulator_MacOSApp: App
	{
    @StateObject var app_state = AppState()
	@State var pickerChoice  = 1.0
	var body: some Scene
		{
		WindowGroup
			{
			ContentView(app_state: app_state)
				.navigationTitle("6809 FLEX")
			}
			.commands
				{
				CommandMenu("Custom Menu")
					{
					Picker("Screen Size", selection: $pickerChoice)
						{
						Text("0.5x").tag(0.5)
						Text("1.0x").tag(1.0)
						Text("1.5x").tag(1.5)
						Text("2.0x").tag(2.0)
						}
						.onReceive([self.pickerChoice].publisher.first())
							{ value in
							print(value)
							if app_state.screen_size != CGFloat(value)
								{
								app_state.screen_size = CGFloat(value)
								}
							}

					Button(action:
						{
						print("Menu Button selected")
						}, label:
						{
						Text("Menu Button")
						})
					}
				}
		}
	}

class AppState : ObservableObject
	{
	@Published var screen_size: CGFloat = 1.0
	}

