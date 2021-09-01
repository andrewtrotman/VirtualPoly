//
//  Acknowledgements.swift
//  Flex Emulator MacOS
//
//  Created by Andrew Trotman on 1/09/21.
//

import Foundation
import SwiftUI

struct Acknowledgements: View
	{
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>

	var body: some View
		{
		VStack
			{
			Spacer()
			Text("Includes Usim by R.P.Bellis")
			Button("OK")
				{
				self.presentationMode.wrappedValue.dismiss()
				}
			Spacer()
			}
			.frame(width: 200, height: 200)
		}
	}



