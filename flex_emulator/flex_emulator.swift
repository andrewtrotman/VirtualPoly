//
//  flex_emulator.swift
//
//  Created by Andrew Trotman on 6/06/21.
//

import SwiftUI

@main
struct flex_emulator: App
	{
	@Environment(\.scenePhase) var scene_phase

	var body: some Scene
		{
		WindowGroup
			{
			ContentView()
			}
		.onChange(of: scene_phase)
			{ newScenePhase in
			switch newScenePhase
				{
				case .active:
					print("App is active")
				case .inactive:
					print("App is inactive")
				case .background:
					print("App is in background")
				@unknown default:
					print("Oh - interesting: I received an unexpected new value.")
				}
			}
		}
	}
