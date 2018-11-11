//
//  GroupString.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/9/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

extension String {
    func group(of n: Int) -> [String] {
        let chars = Array(self)
        return stride(from: 0, to: chars.count, by: n).map {
            String(chars[$0..<min($0+n, chars.count)])
        }
    }
}
