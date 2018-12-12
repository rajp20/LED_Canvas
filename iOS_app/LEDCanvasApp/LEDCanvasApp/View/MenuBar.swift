//
//  MenuBar.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/21/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class MenuBar : UIView, UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout {
   
    lazy var collectionView : UICollectionView = {
        let layout = UICollectionViewFlowLayout()
        let cv = UICollectionView(frame: .zero, collectionViewLayout: layout)
        cv.backgroundColor = UIColor.clear
        cv.dataSource = self
        cv.delegate = self
        return cv
    }()
    
    weak var delegate  : UARTModuleViewController?
    
    let titles = ["Color", "Pattern", "Reset"]
    let icons  = ["ColorSelect", "PatternSelect", "ResetSelect"]
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupCollectionView()
    }
    
    private func setupCollectionView() {
        self.addSubview(collectionView)
        collectionView.register(MenuCell.self, forCellWithReuseIdentifier: "MenuCell")
        
        collectionView.translatesAutoresizingMaskIntoConstraints = false
        collectionView.topAnchor.constraint(equalTo: topAnchor, constant: 10).isActive = true
        collectionView.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -10).isActive = true
        collectionView.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 10).isActive = true
        collectionView.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -10).isActive = true
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    /*********************** UICollectionView Protocols ***********************/
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return 3
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "MenuCell", for: indexPath)
        //Add title to the given cell
        let label = UILabel(frame: CGRect(x: 0, y: (cell.center.y + cell.center.y / 10), width: cell.frame.width, height: cell.frame.height / 3))
        label.text = titles[indexPath.item]
        label.textAlignment = NSTextAlignment.center
        label.textColor = UIColor(red: 0.29, green: 1.0, blue: 0.71, alpha: 1.0)
        label.font = UIFont(name: "Avenir", size: 24)
        
        
        let image = UIImage(named: icons[indexPath.item])
        let icon = UIImageView(frame: CGRect(x: cell.frame.width / 2 - image!.size.width / 3, y: cell.center.y * 0.15 , width: 60.0, height: 60.0))
        icon.image = UIImage(cgImage: (image?.cgImage)!)
        
        cell.addSubview(icon)
        cell.addSubview(label)
        return cell
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        return CGSize(width: collectionView.frame.width / 3.33, height: collectionView.frame.height)
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumInteritemSpacingForSectionAt section: Int) -> CGFloat {
        return 0
    }
    
    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        
        let cellID = indexPath.item
        
        switch cellID {
        case 0:
            let colorVC = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ColorController") as! ColorController
            colorVC.uartVC = delegate
            delegate?.navigationController?.pushViewController(colorVC, animated: true)
        case 1:
            let patternVC = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "PatternController") as? PatternController
            patternVC!.uartVC = delegate
            delegate?.navigationController?.pushViewController(patternVC!, animated: true)
        case 2:
            delegate?.clearContents()
            delegate?.resetButtons = true
            delegate?.patternInProgress = false
        default:
            return
        }
    }
}

