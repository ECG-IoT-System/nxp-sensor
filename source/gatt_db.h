PRIMARY_SERVICE(service_gatt, gBleSig_GenericAttributeProfile_d)
        CHARACTERISTIC(char_service_changed, gBleSig_GattServiceChanged_d, (gGattCharPropRead_c | gGattCharPropNotify_c) )
            VALUE(value_service_changed, gBleSig_GattServiceChanged_d, (gPermissionNone_c), 4, 0x00, 0x00, 0x00, 0x00)
            CCCD(cccd_service_changed)

PRIMARY_SERVICE(service_gap, gBleSig_GenericAccessProfile_d)
    CHARACTERISTIC(char_device_name, gBleSig_GapDeviceName_d, (gGattCharPropRead_c) )
            VALUE(value_device_name, gBleSig_GapDeviceName_d, (gPermissionFlagReadable_c), 12, "NXP_BLE_QPPS")
    CHARACTERISTIC(char_appearance, gBleSig_GapAppearance_d, (gGattCharPropRead_c) )
            VALUE(value_appearance, gBleSig_GapAppearance_d, (gPermissionFlagReadable_c), 2, UuidArray(gGenericHeartrateSensor_c))
    CHARACTERISTIC(char_ppcp, gBleSig_GapPpcp_d, (gGattCharPropRead_c) )
            VALUE(value_ppcp, gBleSig_GapPpcp_d, (gPermissionFlagReadable_c), 8, 0x0A, 0x00, 0x10, 0x00, 0x64, 0x00, 0xE2, 0x04)
//
PRIMARY_SERVICE(service_qpps, uuid_service_qpps_test)
		CHARACTERISTIC(name_readtime, readtime, (gGattCharPropRead_c) )
			VALUE(value_readtime, readtime, (gPermissionFlagReadable_c), 2, 1)
			DESCRIPTOR(test, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
		CHARACTERISTIC(name_writetime, writetime, (gGattCharPropNotify_c))
			VALUE(value_writetime, writetime, (gPermissionNone_c), 1,0x5A )
		CCCD(cccd_qpps_tx)
			DESCRIPTOR(test2, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
		CHARACTERISTIC(name_reqdata, reqdata, (gGattCharPropRead_c) )
			VALUE(value_reqdata, reqdata, (gPermissionFlagReadable_c), 2, 4)
			DESCRIPTOR(test3, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
		CHARACTERISTIC(name_ecgdata, ecgdata, (gGattCharPropNotify_c) )
			VALUE(value_ecgdata, ecgdata, (gPermissionFlagReadable_c),1,0x5A)
		CCCD(cccd_qpps_nx)
			DESCRIPTOR(test4, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)


//debug3 set new service
//PRIMARY_SERVICE_UUID128(service_qpps, uuid_service_qpps)
	//CHARACTERISTIC_UUID128(name_readtime, readtime, (gGattCharPropRead_c) )
//		VALUE_UUID128_VARLEN(value_readtime, readtime, (gPermissionFlagReadable_c), 2, 1)
//		DESCRIPTOR(test, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
//	CHARACTERISTIC_UUID128(name_writetime, writetime, (gGattCharPropNotify_c))
	//	VALUE_UUID128_VARLEN(value_writetime, writetime, (gPermissionNone_c), 2, 200, 0x00, 0xB4)
	//CCCD(cccd_qpps_tx)
	//	DESCRIPTOR(test2, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
	//CHARACTERISTIC_UUID128(name_reqdata, reqdata, (gGattCharPropRead_c) )
	//	VALUE_UUID128_VARLEN(value_reqdata, reqdata, (gPermissionFlagReadable_c), 2, 4)
	//	DESCRIPTOR(test3, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
	//CHARACTERISTIC_UUID128(name_ecgdata, ecgdata, (gGattCharPropNotify_c) )
	//	VALUE_UUID128_VARLEN(value_ecgdata, ecgdata, (gPermissionFlagReadable_c), 512, 4, 0x00)
	//CCCD(cccd_qpps_nx)
	//	DESCRIPTOR(test4, gBleSig_CharUserDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)


//PRIMARY_SERVICE_UUID128(service_qpps, uuid_service_qpps)
	//CHARACTERISTIC_UUID128(name_readtime, readtime, (gGattCharPropRead_c) )
		//VALUE_UUID128_VARLEN(value_readtime, readtime, (gPermissionFlagReadable_c), 512, 1, 0x00)
	//CHARACTERISTIC_UUID128(name_writetime, writetime, (gGattCharPropRead_c))
		//VALUE_UUID128_VARLEN(value_writetime, writetime, (gPermissionFlagReadable_c), 512, 2, 0x00, 0xB4)
	//CHARACTERISTIC_UUID128(name_reqdata, reqdata, (gGattCharPropNotify_c) )
		//VALUE_UUID128_VARLEN(value_reqdata, reqdata, (gPermissionNone_c), 512, 1, 0x00)
	//CHARACTERISTIC_UUID128(char_qpps_tx, uuid_qpps_characteristics_tx, (gGattCharPropNotify_c) )
		//VALUE_UUID128_VARLEN(value_qpps_tx, uuid_qpps_characteristics_tx, (gPermissionNone_c), 512, 1, 0x00)
	//CCCD(cccd_qpps_tx)


//PRIMARY_SERVICE_UUID128(service_qpps, uuid_service_qpps)
   // CHARACTERISTIC_UUID128(char_qpps_rx, uuid_qpps_characteristics_rx, (gGattCharPropWriteWithoutRsp_c) )
     //   VALUE_UUID128_VARLEN(value_qpps_rx, uuid_qpps_characteristics_rx, (gPermissionFlagWritable_c), 512, 1, 0x00)
  //  CHARACTERISTIC_UUID128(char_qpps_tx, uuid_qpps_characteristics_tx, (gGattCharPropNotify_c))
     //   VALUE_UUID128_VARLEN(value_qpps_tx, uuid_qpps_characteristics_tx, (gPermissionNone_c), 512, 2, 0x00, 0xB4)
     //   CCCD(cccd_qpps_tx)

PRIMARY_SERVICE(service_battery, gBleSig_BatteryService_d)
    CHARACTERISTIC(char_battery_level, gBleSig_BatteryLevel_d, ( gGattCharPropRead_c))
        VALUE(value_battery_level, gBleSig_BatteryLevel_d, (gPermissionFlagReadable_c), 1, 0x5A)
        DESCRIPTOR(desc_bat_level, gBleSig_CharPresFormatDescriptor_d, (gPermissionFlagReadable_c), 7, 0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00)
        CCCD(cccd_battery_level)

PRIMARY_SERVICE(service_device_info, gBleSig_DeviceInformationService_d)
    CHARACTERISTIC(char_manuf_name, gBleSig_ManufacturerNameString_d, (gGattCharPropRead_c) )
        VALUE(value_manuf_name, gBleSig_ManufacturerNameString_d, (gPermissionFlagReadable_c), sizeof(MANUFACTURER_NAME), MANUFACTURER_NAME)
    CHARACTERISTIC(char_model_no, gBleSig_ModelNumberString_d, (gGattCharPropRead_c) )
        VALUE(value_model_no, gBleSig_ModelNumberString_d, (gPermissionFlagReadable_c), 9, "QPPS Demo")
    CHARACTERISTIC(char_serial_no, gBleSig_SerialNumberString_d, (gGattCharPropRead_c) )
        VALUE(value_serial_no, gBleSig_SerialNumberString_d, (gPermissionFlagReadable_c), 7, "BLESN01")
    CHARACTERISTIC(char_hw_rev, gBleSig_HardwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_hw_rev, gBleSig_HardwareRevisionString_d, (gPermissionFlagReadable_c), sizeof(BOARD_NAME), BOARD_NAME)
    CHARACTERISTIC(char_fw_rev, gBleSig_FirmwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_fw_rev, gBleSig_FirmwareRevisionString_d, (gPermissionFlagReadable_c), 5, "1.1.1")
    CHARACTERISTIC(char_sw_rev, gBleSig_SoftwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_sw_rev, gBleSig_SoftwareRevisionString_d, (gPermissionFlagReadable_c), 5, "1.1.4")
    CHARACTERISTIC(char_system_id, gBleSig_SystemId_d, (gGattCharPropRead_c) )
        VALUE(value_system_id, gBleSig_SystemId_d, (gPermissionFlagReadable_c), 8, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x9F, 0x04, 0x00)
    CHARACTERISTIC(char_rcdl, gBleSig_IeeeRcdl_d, (gGattCharPropRead_c) )
        VALUE(value_rcdl, gBleSig_IeeeRcdl_d, (gPermissionFlagReadable_c), 4, 0x00, 0x00, 0x00, 0x00)

