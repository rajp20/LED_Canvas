//
//  Queue.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/17/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class Queue<T> {
    
    fileprivate var q    : [Any]!
    fileprivate var size : Int!
    
    init() {
        q = [Any]()
        size = 0
    }
    
    public func enqueue<T>(_ element: T) {
        size += 1
        q.append(element)
    }
    
    public func dequeue<T>() -> T? {
        size -= 1
        guard !q.isEmpty, let element = q.first else { return nil }
        
        q.removeFirst()
        
        return element as? T
    }
    
    public func isEmpty() -> Bool {
        return q.isEmpty
    }
    
    public func peek<T>() -> T {
        return q.first as! T
    }
}

/********* Extension to print contents of the Queue object *********/

extension Queue : CustomStringConvertible {
    
    public var description : String {
        return q.description
    }
}
