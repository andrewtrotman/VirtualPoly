//
//  Compatibility.swift
//  Flex Emulator MacOS
//
//  Created by Andrew Trotman on 28/08/21.
//
// Taken from: https://www.swiftbysundell.com/tips/making-uiimage-macos-compatible/

// Either put this in a separate file that you only include in your macOS target or wrap the code in #if os(macOS) / #endif

import Cocoa

// Step 1: Typealias UIImage to NSImage
typealias UIImage = NSImage

// Step 2: You might want to add these APIs that UIImage has but NSImage doesn't.
extension NSImage {
    var cgImage: CGImage? {
        var proposedRect = CGRect(origin: .zero, size: size)

        return cgImage(forProposedRect: &proposedRect,
                       context: nil,
                       hints: nil)
    }
}

// Step 3: Profit - you can now make your model code that uses UIImage cross-platform!
struct User {
    let name: String
    let profileImage: UIImage
}