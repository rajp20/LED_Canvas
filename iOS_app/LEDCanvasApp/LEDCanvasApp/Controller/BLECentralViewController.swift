//
//  BLECentralViewController.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 9/14/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import Foundation
import UIKit
import CoreBluetooth

var txCharacteristic : CBCharacteristic?
var rxCharacteristic : CBCharacteristic?
var blePeripheral : CBPeripheral?
var characteristicASCIIValue = NSString()

class BLECentralViewController : UIViewController, CBCentralManagerDelegate, CBPeripheralDelegate, UITableViewDelegate, UITableViewDataSource {
    
    var centralManager : CBCentralManager!
    var RSSIs = [NSNumber]()
    var data = NSMutableData()
    var writeData: String = ""
    var peripherals: [CBPeripheral] = []
    var characteristicValue = [CBUUID: NSData]()
    var timer = Timer()
    var characteristics = [String : CBCharacteristic]()
    
    weak var delegate : UARTModuleViewController?
    
    //Consider making a custom class for the peripheral table
    @IBOutlet weak var peripheralsTable : UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.peripheralsTable.delegate = self
        self.peripheralsTable.dataSource = self
        self.peripheralsTable.reloadData()
        
        /* Our key player in this app will be our CBCentralManager. CBCentralManager objects are used to manage discovered or
         * connected remote peripheral devices (represented by CBPeripheral objects), including scanning for, discovering, and
         * connecting to advertising peripherals.
         */
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        disconnectFromDevice()
        super.viewDidAppear(animated)
        refreshScanView()
        print("View Cleared")
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        print("Stop Scanning")
        centralManager?.stopScan()
    }
    
    func refreshScanView() {
        peripheralsTable.reloadData()
    }
    
    /****************************************************************
     *                    SCANNING FOR PERIPHERALS                  *
     ****************************************************************/
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        
        // We will just handle it the easy way here: if Bluetooth is on, proceed...start scan!
        if central.state == CBManagerState.poweredOn {
            print("Bluetooth is enabled")
            startScanForPeripherals()
        } else {
            //If Bluetooth is off, display a UI alert message saying "Bluetooth is not enable" and "Make sure that your bluetooth is turned on"
            print("Bluetooth is not enabled - make sure your Bloothooth is turned on")
            
            let _title   = "Bluetooth Disabled."
            let _message = "Bluetooth has not been enabled. Please make sure that Bluetooth is enabled on your device."
            
            let alertViewController = UIAlertController(title: _title, message: _message, preferredStyle: UIAlertControllerStyle.alert)
            let alertAction         = UIAlertAction(title: "Ok", style: UIAlertActionStyle.default, handler: { (action: UIAlertAction) -> Void in
                self.dismiss(animated: true, completion: nil)
            })
            
            alertViewController.addAction(alertAction)
            self.present(alertViewController, animated: true, completion: nil)
        }
    }
    
    /**
     * This function is called once the centralManager has Bluetooth enabled.  The function begins scanning for peripheral devices for 17
     * seconds.  When the 17 second time interval is exceeded, the timer triggers the centralManager to cancel scanning.
     **/
    func startScanForPeripherals() {
        peripherals = []
        print("Scanning...")
        self.timer.invalidate()
        centralManager.scanForPeripherals(withServices: [BLEService_UUID], options: [CBCentralManagerScanOptionAllowDuplicatesKey:false])
        // This line for timer allows the device to scan for peripherals for 17 seconds. Once the time interval is exceeded, the timer calls
        // self.cancelScan which stops any scanning.
        Timer.scheduledTimer(timeInterval: 17, target: self, selector: #selector(self.cancelScan), userInfo: nil, repeats: false)
    }
    
    /*We also need to stop scanning at some point so we'll also create a function that calls "stopScan"*/
    @objc func cancelScan() {
        self.centralManager?.stopScan()
        print("Scan Stopped")
        print("Number of Peripherals Found: \(peripherals.count)")
    }
    
    func restoreCentralManager() {
        //Restores Central Manager delegate if something went wrong
        centralManager?.delegate = self
    }
    
    /**
     * This function is used for Discovering peripherals once scanning has started. Every time a peripheral is discovered,
     * the CBCentralManager will notify us by calling the centralManager(_:didDiscover:advertisementData:rssi:) function on its delegate.
     *
     * @params:
     *    - central : CBCentralManager       = providing the update
     *    - peripheral : CBPeriphal          = the discovered peripheral as a CBPeripheral object
     *    - advertisementData: [String: Any] = a dictionary containing the peripherals advertisement data
     *    - rssi : NSNumber                  = the current received signal strength indicator (RSSI) of the peripheral, in decibels.
     **/
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        
        /* In our implementation of this function we perform the following actions:
        
            1) Stop scanning for peripherals (for our example app, we're only interested in the first one which appears)
            2) Add the newly discovered peripheral to an array of peripherals.
            3) Add the new peripheral's RSSI to an array of RSSIs.
            4) Set the peripheral's delegate to self (BLECentralViewController)
            5) Tell the Central Manager to discover more details about the peripheral's service
            6) Set the blePeripheral variable to the new peripheral and print relevant information in the debug window
         */
       
        blePeripheral = peripheral
        self.peripherals.append(peripheral)
        self.RSSIs.append(RSSI)
        peripheral.delegate = self
        self.peripheralsTable.reloadData()
        if blePeripheral == nil {
            print("Found new pheripheral devices with services")
            print("Peripheral name: \(String(describing: peripheral.name))")
            print("**********************************")
            print ("Advertisement Data : \(advertisementData)")
        }
    }
    
    /****************************************************************
     *                  CONNECTIONS TO PERIPHERAL                   *
     ****************************************************************/
    
    /**
     * The connectToDevice() function calls centralManager?.connect to establishe a local connection to the desired peripheral.
     * Once the connection is made, the central manager calls the centralManager(_:didConnect) delegate function to provide incoming
     * information about the newly connected peripheral.
     **/
    func connectToDevice() {
        centralManager.connect(blePeripheral!, options: nil)
    }
    
    /**
     * This is a delegate function called by the centralManager after a conncetion to a peripheral is made. This function will display
     * the selected peripherals info to the console, stop scanning for peripherals, erase any data from previous scans, set the
     * peripherals delegate, and discover the peripherals services.
     *
     * @params:
     *    - central : CBCentralManager = provides the update
     *    - peripheral : CBPeripheral  = the peripheral that the local device has connected to
     **/
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("*****************************")
        print("Connection complete")
        print("Peripheral info: \(String(describing: blePeripheral))")
        
        //Stop Scan- We don't need to scan once we've connected to a peripheral. We got what we came for.
        centralManager?.stopScan()
        print("Scan Stopped")
        
        //Erase data that we might have
        data.length = 0
        
        //Discovery callback
        peripheral.delegate = self
        //Only look for services that matches transmit uuid
        peripheral.discoverServices([BLEService_UUID])
        
        //Once connected, move to new view controller to manager incoming and outgoing data
        let storyboard = UIStoryboard(name: "Main", bundle: nil)
        
        let uartViewController = storyboard.instantiateViewController(withIdentifier: "UARTModuleViewController") as! UARTModuleViewController
        delegate = uartViewController
        uartViewController.peripheral = peripheral
        
        navigationController?.pushViewController(uartViewController, animated: true)
    }
    
    /**
     * Once the peripheral's services are successfully discovered, the centralManager will call the didDiscoverServices() delegate
     * function. didDiscoverService() handles and filters services, so that we can use whichever service we are interested in right away.
     * The function first handles any possible errors returned by the central manager, then we request characteristics for each service
     * returned by calling discoverCharacteristics(_:)
     *
     * @params:
     *      - peripheral : CBPeripheral = provides the update
     *      - error : Error             = object that contains an error if one is triggered, otherwise nil
     **/
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("*******************************************************")
        
        if ((error) != nil) {
            print("Error discovering services: \(error!.localizedDescription)")
            return
        }
        
        guard let services = peripheral.services else {
            return
        }
        //We need to discover the all characteristic
        for service in services {
            
            peripheral.discoverCharacteristics(nil, for: service)
        }
        print("Discovered Services: \(services)")
    }
    
    /**
     * Now that we've called the discoverCharacteristics(_:) function, the central manager will call the didDiscoverCharacteristicsFor()
     * delegate function and provide the discovered characteristics of the specified service. This function handles errors and print
     * characteristic info to the debug console, looks through the array of characteristics for a match to our desired UUIDs, performs
     * any necessary actions for the matching characteristics, and discovers descriptors for each characteristic.
     *
     * In this case, the specific UUIDs we're looking for are stored in the BLE_Characteristic_uuid_Rx and BLE_Characteristic_uuid_Tx
     * variables (found in UUIDKey.swift). When we find the RX characteristic, we subscribe to updates to its value by calling
     * setNotifyValue() - this is how we receive data from the peripheral. Additionally, we read the current value from the characteristic
     * and print its info to the console. When we find the TX characteristic, we save a reference to it so we can write values to it later
     * - this is how we send data to the peripheral.
     *
     * @params:
     *    - peripheral : CBPeripheral = provides the update
     *    - service : CBService       = service containing characteristics (look for match to our desired UUIDs.)
     *    - error : Error             = error object will contain error or be nil
     **/
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        
        print("*******************************************************")
        
        if ((error) != nil) {
            print("Error discovering services: \(error!.localizedDescription)")
            return
        }
        
        guard let characteristics = service.characteristics else {
            return
        }
        
        print("Found \(characteristics.count) characteristics!")
        
        for characteristic in characteristics {
            //looks for the right characteristic
            
            if characteristic.uuid.isEqual(BLE_Characteristic_uuid_Rx)  {
                rxCharacteristic = characteristic
                
                //Once found, subscribe to this particular characteristic...
                peripheral.setNotifyValue(true, for: rxCharacteristic!)
                // We can return after calling CBPeripheral.setNotifyValue because CBPeripheralDelegate's
                // didUpdateNotificationStateForCharacteristic method will be called automatically
                peripheral.readValue(for: characteristic)
                print("Rx Characteristic: \(characteristic.uuid)")
            }
            if characteristic.uuid.isEqual(BLE_Characteristic_uuid_Tx){
                txCharacteristic = characteristic
                print("Tx Characteristic: \(characteristic.uuid)")
            }
            peripheral.discoverDescriptors(for: characteristic)
        }
    }
    
    // Getting Values From Characteristic
    func peripheral(_ peripheral: CBPeripheral, didDiscoverDescriptorsFor characteristic: CBCharacteristic, error: Error?) {
        print("*******************************************************")
        
        if error != nil {
            print("\(error.debugDescription)")
            return
        }
        if ((characteristic.descriptors) != nil) {
            
            for x in characteristic.descriptors!{
                let descript = x as CBDescriptor?
                print("function name: DidDiscoverDescriptorForChar \(String(describing: descript?.description))")
                print("Rx Value \(String(describing: rxCharacteristic?.value))")
                print("Tx Value \(String(describing: txCharacteristic?.value))")
            }
        }
    }
    
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
        print("*******************************************************")
        
        if (error != nil) {
            print("Error changing notification state:\(String(describing: error?.localizedDescription))")
            
        } else {
            print("Characteristic's value subscribed")
        }
        
        if (characteristic.isNotifying) {
            print ("Subscribed. Notification has begun for: \(characteristic.uuid)")
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        print("Disconnected")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor descriptor: CBDescriptor, error: Error?) {
        guard error == nil else {
            print("Error discovering services: error")
            return
        }
        print("Succeeded!")
    }
    
    /**
     * This function is called when the user wants to disconnect (via button for example), the disconnectFromDevice() function is called.
     * The disconnectFromDevice() function first checks if there is a current peripheral set - if there is, it calls
     * cancelPeripheralConnection() which cancels an active or pending local connection to the peripheral.
     **/
    func disconnectFromDevice () {
        if blePeripheral != nil {
            centralManager?.cancelPeripheralConnection(blePeripheral!)
        }
    }
    
    /**
     * This function calls .cancelPeripheralConnection which states the following in documentation:
     *
     * This method is nonblocking, and any CBPeripheral class commands that are still pending to peripheral may or may not complete. Because
     * other apps may still have a connection to the peripheral, canceling a local connection does not guarantee that the underlying physical
     * link is immediately disconnected. From the app’s perspective, however, the peripheral is considered disconnected, and the central manager
     * object calls the centralManager:didDisconnectPeripheral:error: method of its delegate object.
     **/
    func disconnectAllConnection() {
        centralManager.cancelPeripheralConnection(blePeripheral!)
    }
    
    /****************************************************************
     *                         COMMUNICATION                        *
     ****************************************************************/
    
    /**
     * Because we called peripheral.setNotify() in the previous step, whenever new data is available on the RX characteristic, two functions
     * will be called onthe peripheral's delegate –peripheral(_:didUpdateNotificationStateFor:error:) & peripheral(_:didUpdateValueFor:error:).
     *
     * In our implementation, peripheral(_:didUpdateNotificationStateFor:error:) simply prints info to the console. peripheral(_:didUpdateValueFor:error:)
     * is where we read and handle the new incoming data from the peripheral.
     *
     * In didUpdateValueFor(), we check to make sure the characteristic is the RX characteristic.  If it is, we read the value and convert it to an ASCII
     * string.  We print the converted ASCII value to the console and we post a notification which can be used to update UI elements with the newly received
     * data.
     *
     * @params:
     *    - peripheral : CBPeripheral         = provides the update
     *    - characteristic : CBCharacteristic = used to check if the characteristic is an Rx characteristic
     *    - error : Error                     = error object will contain error or be nil
     **/
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        
        if characteristic == rxCharacteristic {
            if let ASCIIstring = NSString(data: characteristic.value!, encoding: String.Encoding.utf8.rawValue) {
                characteristicASCIIValue = ASCIIstring
                print("Value Recieved: \((characteristicASCIIValue as String))")
                NotificationCenter.default.post(name:NSNotification.Name(rawValue: "Notify"), object: nil)
                
                // Check to see if a reset needs to be sent, as it has priority over everything
                if (delegate?.shouldReset == true){
                    delegate?.writeValue(data: "0")
                    delegate?.shouldReset = false
                    delegate?.resetButtons = true
                }
                else{
                    if !(delegate?.patterns!.ballPatternInProgress)! || !(delegate?.patterns!.ripplePatternInProgress)! || !(delegate?.patterns!.acidPatternInProgress)! || (delegate?.patterns!.lifePatternInProgress)! {
                        if !((delegate?.dataQueue!.isEmpty())!) {
                            delegate?.idleState = false
                            let coordinate = delegate?.coordinateString(point: delegate!.dataQueue.dequeue())
                            delegate?.writeValue(data: coordinate!)
                        }
                        else {
                            delegate!.idleState = true
                        }
                    }
                }
            }
        }
    }
    
    /**
     * Since the CBCharacteristicWriteType is using the .withResponse property, this function will notify us of the response from the peripheral.
     *
     * @params:
     *    - peripheral : CBPeripheral         = provides the update
     *    - characteristic : CBCharacteristic = the characteristic, which should be Tx characteristic
     *    - error : Error                     = error object will contain error or be nil
     **/
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        guard error == nil else {
            print("Error discovering services: error")
            return
        }
    }
    
    /****************************************************************
     *                          TABLEVIEW                           *
     ****************************************************************/
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.peripherals.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        //Connect to device where the peripheral is connected
        let cell = tableView.dequeueReusableCell(withIdentifier: "PeripheralCell") as! PeripheralCell
        let peripheral = self.peripherals[indexPath.row]
        let RSSI = self.RSSIs[indexPath.row]
        
        if peripheral.name == nil {
            cell.peripheralLabel.text = "nil"
        } else {
            cell.peripheralLabel.text = peripheral.name
        }
        cell.rssiLabel.text = "RSSI: \(RSSI)"
        
        return cell
    }
    
    /**
     * Connects to a peripheral after a user clicks on a row (cell) in the table view.
     **/
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {

        blePeripheral = peripherals[indexPath.row]
        connectToDevice()
    }
}
