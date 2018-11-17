//
//  MenuCell.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/16/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class MenuCell: UICollectionViewCell {
    
    @IBOutlet var cellImage: UIImageView!
    @IBOutlet var cellLabel: UILabel!
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self.backgroundColor = UIColor.blue
        self.layer.cornerRadius = 10.0
        cellLabel = UILabel(frame: CGRect(x: 0, y: frame.size.height, width: frame.width, height: frame.size.height * 0.3))
//        cellLabel.text = "Hello"
//        addSubview(cellLabel)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
