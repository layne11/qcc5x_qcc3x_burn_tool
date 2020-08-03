/*******************************************************************************

    FILE:       TestFlash.h

                Copyright (c) 2001-2019 Qualcomm Technologies International, Ltd.
                All Rights Reserved.
                Qualcomm Technologies International, Ltd. Confidential and Proprietary.

    PURPOSE:    Defines the TestFlash DLL API, used for programming Flash and
                other supported non-volatile memory (NVM) devices.

*******************************************************************************/

#ifndef __INCTestFlashh
#define __INCTestFlashh

#define TESTFLASH_API(T) T _stdcall

//#include "types.h"

#ifndef COMMON__TYPES_H
typedef unsigned char  uint8;
typedef signed char   int8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned long int uint32;
typedef signed long int int32;
#endif /* COMMON__TYPES_H */

/* Return codes */
#define TFL_OK 0
#define TFL_ERROR -1
#define TFL_DEVICE_ERROR -2
#define TFL_ERROR_UNSUPPORTED -3

#define TFL_SPI_LPT 0x1
#define TFL_SPI_USB 0x2
#define TFL_TRB     0x3
#define TFL_USBDBG  0x4

#define TFL_CHIP 0
#define TFL_FILE 1

#define TFL_ERROR_OPEN_FAILED      0x1001
#define TFL_ERROR_DEVICE_OPEN      0x1002
#define TFL_ERROR_DEVICE_NOT_OPEN  0x1003
#define TFL_ERROR_DEVICE_BUSY      0x1004
#define TFL_ERROR_THREAD_ERROR     0x1005
#define TFL_ERROR_RESET_FAIL       0x1006
#define TFL_ERROR_WRONG_TRANS      0x1008


#define TFL_ERROR_SPIUNLOCK        0x1009 /* Indicates an error connected with transport unlocking */

#define TFL_MAX_DEVICES 32
#define TFL_ALL_DEVICES 0xFFFFFFFF

/* non-volatile memory types (FLASH or similar) used by flSetFlashType / flmSetFlashType */
#define TFL_TYPE_STANDARD 0x0
#define TFL_TYPE_SPIF     0x1
#define TFL_TYPE_SQIF     0x2
#define TFL_TYPE_MTP      0x3
#define TFL_TYPE_OTP      0x4
#define TFL_TYPE_SMEM     0x5
#define TFL_TYPE_E2       0x6

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*************************************************************************************

    Function :      int32 flOpen(int32 port, int32 xtal, int32 delays,
                                 int32 transport)

    Parameters :    port -
                        For USB or LPT transports:
                        Defines the physical port to use. An LPT port number starting
                        from 1, or the id number of a USB SPI device. Set to -1 to
                        use the default (port 1 if LPT, first USB SPI device if USB).
                        NOTE: Default may be overridden by the "SPIPORT" environment
                        variable.
                        <p>
                        For TRB or USBDBG transports:
                        For TRB, the serial number of the usb2trb converter e.g.
                        "123456" may be used. For USBDBG, the port identifier of the
                        corresponding USB Hub Filter (e.g. "100") may be used.
                        Alternatively, a sequence number ranging from 1 to the number
                        of connected devices may be used for TRB and USBDBG. For
                        further details of TRB and USBDBG device identifiers, refer
                        to CS-00410202-UG.

                    xtal -
                        Specifies module's crystal frequency. Acceptable values are
                        10, 16, 26, 32. Higher values make code run slower, lower
                        values may cause instability. (Casiras use 16Mhz).
                        <p>
                        This parameter is only applicable to BlueCore ICs.

                    delays -
                        For transports types TFL_SPI_LPT or TFL_SPI_USB, this
                        represents the delay used in driving the SPI port.
                        1 = fastest SPI speed, but may give unreliability.
                        2 = reasonable value, higher values may be better if chip is
                        in deep sleep.
                        <p>
                        For other transport types this value is unused.

                    transport -
                        Defines the transport to be used, where:
                            <table>
                                <tr><td>TFL_SPI_LPT <td>1
                                <tr><td>TFL_SPI_USB <td>2
                                <tr><td>TFL_TRB     <td>3
                                <tr><td>TFL_USBDBG  <td>4
                            </table>
                        Set to -1 to use the default (from SPITRANS environment
                        variable if present), otherwise LPT.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is one of the flOpen* set of functions used to
                    create the appropriate host side objects to enable NVM
                    programming via a single debug or gang programming interface.
                    <p>
                    To be used (only) with devices which are not transport locked.
                    flOpenTrans can also be used to do this.
                    <p>
                    flOpenUnlock or flOpenUnlockTrans should be used with transport
                    locked devices.
                    <p>
                    This is an entry point to the single connection version of
                    TestFlash. This can be used when programming a single device, and
                    must be used when using a parallel port gang programmer with
                    devices which are not transport locked.
                    <p>
                    In the case of a gang programmer being used, this function does
                    not verify whether the chips can be communicated with.

*************************************************************************************/
TESTFLASH_API(int32) flOpen(int32 port, int32 xtal, int32 delays, int32 transport);

/*************************************************************************************

    Function :      int32 flOpenUnlock(int32 port, int32 xtal, int32 delays,
                                       int32 transport, const char* unlockKey)

    Parameters :    port -
                        For USB or LPT transports:
                        Defines the physical port to use. An LPT port number starting
                        from 1, or the id number of a USB SPI device. Set to -1 to
                        use the default (port 1 if LPT, first USB SPI device if USB).
                        NOTE: Default may be overridden by the "SPIPORT" environment
                        variable.
                        <p>
                        For TRB or USBDBG transports:
                        For TRB, the serial number of the usb2trb converter e.g.
                        "123456" may be used. For USBDBG, the port identifier of the
                        corresponding USB Hub Filter (e.g. "100") may be used.
                        Alternatively, a sequence number ranging from 1 to the number
                        of connected devices may be used for TRB and USBDBG. For
                        further details of TRB and USBDBG device identifiers, refer
                        to CS-00410202-UG.

                    xtal -
                        Specifies module's crystal frequency. Acceptable values are
                        10, 16, 26, 32. Higher values make code run slower, lower
                        values may cause instability. (Casiras use 16Mhz).
                        <p>
                        This parameter is only applicable to BlueCore ICs.

                    delays -
                        For transports types TFL_SPI_LPT or TFL_SPI_USB, this
                        represents the delay used in driving the SPI port.
                        1 = fastest SPI speed, but may give unreliability.
                        2 = reasonable value, higher values may be better if chip is
                        in deep sleep.
                        <p>
                        Where transport = TFL_USBDBG, this parameter will define how
                        long the function will retry, in ms, when trying to detect if
                        the specified USBDBG device has enumerated. It has been found
                        that some PC's take a longer time than expected to enumerate a
                        USB device and, in some cases, the PC's internal USB hub may
                        have been reset which can cause a further delay in enumeration.
                        This parameter can be set to accommodate the extra time taken
                        to reset USB devices.
                        <p>
                        For other transport types this value is unused.

                    transport -
                        Defines the transport to be used, where:
                            <table>
                                <tr><td>TFL_SPI_LPT <td>1
                                <tr><td>TFL_SPI_USB <td>2
                                <tr><td>TFL_TRB     <td>3
                                <tr><td>TFL_USBDBG  <td>4
                                </table>
                        Set to -1 to use the default (from SPITRANS environment
                        variable if present), otherwise LPT.

                    unlockKey -
                        Defines the unlock key. By default, the unlock key should be
                        supplied unencrypted. For USBDBG only, the user can override
                        the default and supply an encrypted key - this must be
                        prefaced with 'ENC', for example,
                        "ENCdc0ed85df9611abb7249cdd168c5467e".

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is one of the flOpen* set of functions used to
                    create the appropriate host side objects to enable NVM
                    programming via a single debug or gang programming interface.
                    <p>
                    To be used (only) with transport locked devices.
                    flOpenUnlockTrans can also be used to do this.
                    <p>
                    flOpen or flOpenTrans should be used with devices which are not
                    transport locked.
                    <p>
                    This is an entry point to the single connection version of
                    TestFlash. This can be used when programming a single device, and
                    must be used when using a parallel port gang programmer with
                    transport locked devices.
                    <p>
                    In the case of a gang programmer being used, this function does
                    not verify whether the chips can be communicated with.

*************************************************************************************/
TESTFLASH_API(int32) flOpenUnlock(int32 port, int32 xtal, int32 delays,
                                  int32 transport, const char* unlockKey);

/*************************************************************************************

    Function :      int32 flmOpen(uint32 deviceMask, int32 xtal, int32 transport);

    Parameters :    deviceMask -
                        A bit mask of the devices to be opened. For example, to open
                        devices 2 and 4 using USB-SPI, TRB or USBDBG connections, a
                        device mask of 0x0A would be used.
                        <p>
                        NOTE: Devices are opened according to their port number, i.e.
                        in the case of USB-SPI, the order of enumeration, where bit 0
                        being set means that port 0 will be opened. For LPT-SPI, port
                        numbers start at 1, therefore bit 0 is unused, and bit 1 must
                        be set to open LPT1. For TRB and USBDBG transports, the bit
                        positions refer to the order of the adapter serial numbers
                        (for USBDBG, the port identifiers of the corresponding USB Hub
                        Filters), with bit 0 being the first device (port 1, lowest
                        adapter serial number / USB Hub Filter port identifier). For
                        further details of TRB and USBDBG device identifiers, refer
                        to CS-00410202-UG.

                    xtal -
                        See flOpen.

                    transport -
                        See flOpen.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>


    Description:    This is the multiple connection version of flOpen. See flOpen for
                    more details.
                    <p>
                    To be used (only) with devices which are not transport locked.
                    flmOpenTrans can be used as an alternative to flmOpen.
                    <p>
                    flmOpenUnlock or flmOpenUnlockTrans should be used with transport
                    locked devices.
                    <p>
                    This function will fail if the chip(s) cannot be communicated
                    with. If the return value is TFL_DEVICE_ERROR, use
                    flmGetBitErrorField to determine which devices failed or succeeded.
                    <p>
                    This is an entry point for the multiple connection version of
                    TestFlash. This must be used when programming multiple devices
                    which are not transport locked, but not when using a parallel
                    port gang programmer.
                    <p>
                    flmOpen can either be called with a mask specifying all devices to
                    open, or it can be called multiple times to open multiple devices
                    (the devices in the mask must be different for each call). In the
                    case of multiple calls to flmOpen, the transport type must be the
                    same for each call.

*************************************************************************************/
TESTFLASH_API(int32) flmOpen(uint32 deviceMask, int32 xtal, int32 transport);

/*************************************************************************************

    Function :      int32 flmOpenUnlock(uint32 deviceMask, int32 xtal,
                                        int32 transport, const char* unlockKey);

    Parameters :    deviceMask -
                        A bit mask of the devices to be opened. For example, to open
                        devices 2 and 4 using USB-SPI connections, a device mask of
                        0x0A would be used.
                        <p>
                        NOTE: Devices are opened according to their port number, i.e.
                        in the case of USB-SPI, the order of enumeration, where bit 0
                        being set means that port 0 will be opened. For LPT-SPI, port
                        numbers start at 1, therefore bit 0 is unused, and bit 1 must
                        be set to open LPT1. For TRB and USBDBG transports, the bit
                        positions refer to the order of the adapter serial numbers
                        (for USBDBG, the port identifiers of the corresponding USB Hub
                        Filters), with bit 0 being the first device (port 1, lowest
                        adapter serial number / USB Hub Filter port identifier). For
                        further details of TRB and USBDBG device identifiers, refer
                        to CS-00410202-UG.

                    xtal -
                        See flOpen.

                    transport -
                        See flOpen.

                    unlockKey -
                        Defines the unlock key. By default, the unlock key should be
                        supplied unencrypted. For USBDBG only, the user can override
                        the default and supply an encrypted key - this must be
                        prefaced with 'ENC', for example,
                        "ENCdc0ed85df9611abb7249cdd168c5467e".

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>


    Description:    This is the multiple connection version of flOpenUnlock. See
                    flOpenUnlock for more details.
                    <p>
                    To be used (only) with transport locked devices.
                    flmOpenUnlockTrans can be used as an alternative to flmOpenUnlock.
                    <p>
                    flmOpen or flmOpenTrans should be used with devices which are not
                    transport locked.
                    <p>
                    This function will fail if the chip(s) cannot be communicated
                    with. If the return value is TFL_DEVICE_ERROR, use
                    flmGetBitErrorField to determine which devices failed or succeeded.
                    <p>
                    This is an entry point for the multiple connection version of
                    TestFlash. This must be used when programming multiple transport
                    locked devices, but not when using a parallel port gang programmer
                    to do so.
                    <p>
                    flmOpenUnlock can either be called with a mask specifying all
                    devices to open, or it can be called multiple times to open
                    multiple devices (the devices in the mask must be different for
                    each call). In the case of multiple calls to flmOpen, the
                    transport type must be the same for each call.

*************************************************************************************/
TESTFLASH_API(int32) flmOpenUnlock(uint32 deviceMask, int32 xtal,
                                   int32 transport, const char* unlockKey);

/*************************************************************************************

    Function :      int32 flOpenTrans(const char* trans, int32 xtal, int32 delays)

    Parameters :    trans -
                        String of space separated transport options that define the
                        transport to use. Commonly used options include:
                            SPITRANS (The physical transport to use, e.g. LPT, USB,
                                      TRB, USBDBG)
                            SPIPORT (The port number)
                        E.g. for LPT1, trans would be "SPITRANS=LPT SPIPORT=1".
                        These options override any environment variables of the same
                        names.
                        The transport string may be one of those returned by
                        flGetAvailablePorts, which returns transport strings for
                        all available ports.

                    xtal -
                        Specifies module's crystal frequency. Acceptable values are
                        10, 16, 26, 32. Higher values make code run slower, lower
                        values may cause instability. (Casiras use 16Mhz).
                        <p>
                        This parameter is only applicable to BlueCore ICs.

                    delays -
                        For transports types TFL_SPI_LPT or TFL_SPI_USB, this
                        represents the delay used in driving the SPI port.
                        1 = fastest SPI speed, but may give unreliability.
                        2 = reasonable value, higher values may be better if chip is
                        in deep sleep.
                        <p>
                        For other transport types this value is unused.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is one of the flOpen* set of functions used to
                    create the appropriate host side objects to enable NVM
                    programming via a debug or gang programming interface.
                    <p>
                    To be used (only) with devices which are not transport locked.
                    flOpen can also be used to do this.
                    <p>
                    flOpenUnlock or flOpenUnlockTrans should be used with transport
                    locked devices.
                    <p>
                    The trans string may be one of those returned by
                    flGetAvailableports, which returns transport strings for all
                    available ports.
                    <p>
                    In the case of a gang programmer being used, this function does
                    not verify whether the chips can be communicated with.

*************************************************************************************/
TESTFLASH_API(int32) flOpenTrans(const char* trans, int32 xtal, int32 delays);

/*************************************************************************************

    Function :      int32 flOpenUnlockTrans(const char* trans, int32 xtal,
                                            int32 delays, const char* unlockKey)

    Parameters :    trans -
                        String of space separated transport options that define the
                        transport to use. Commonly used options include:
                            SPITRANS (The physical transport to use, e.g. LPT, USB,
                                      TRB, USBDBG)
                            SPIPORT (The port number)
                        E.g. for LPT1, trans would be "SPITRANS=LPT SPIPORT=1".
                        These options override any environment variables of the same
                        names.
                        The transport string may be one of those returned by
                        flGetAvailablePorts, which returns transport strings for
                        all available ports.

                    xtal -
                        Specifies module's crystal frequency. Acceptable values are
                        10, 16, 26, 32. Higher values make code run slower, lower
                        values may cause instability. (Casiras use 16Mhz).
                        <p>
                        This parameter is only applicable to BlueCore ICs.

                    delays -
                        For transports types TFL_SPI_LPT or TFL_SPI_USB, this
                        represents the delay used in driving the SPI port.
                        1 = fastest SPI speed, but may give unreliability.
                        2 = reasonable value, higher values may be better if chip is
                        in deep sleep.
                        <p>
                        Where transport = TFL_USBDBG, this parameter will define how
                        long the function will retry, in ms, when trying to detect if
                        the specified USBDBG device has enumerated. It has been found
                        that some PC's take a longer time than expected to enumerate a
                        USB device and, in some cases, the PC's internal USB hub may
                        have been reset which can cause a further delay in enumeration.
                        This parameter can be set to accommodate the extra time taken
                        to reset USB devices. If USB is not used then this value can
                        be set to 0.
                        <p>
                        For other transport types this value is unused.

                    unlockKey -
                        Defines the unlock key. By default, the unlock key should be
                        supplied unencrypted. For USBDBG only, the user can override
                        the default and supply an encrypted key - this must be
                        prefaced with 'ENC', for example,
                        "ENCdc0ed85df9611abb7249cdd168c5467e".

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is one of the flOpen* set of functions used to
                    create the appropriate host side objects to enable NVM
                    programming via a debug or gang programming interface.
                    <p>
                    To be used (only) with devices which are transport locked.
                    flOpenUnlock can also be used to do this.
                    <p>
                    flOpen or flOpenTrans should be used with devices which are not
                    transport locked.
                    <p>
                    The trans string may be one of those returned by
                    flGetAvailablePorts, which returns transport strings for all
                    available ports.
                    <p>
                    In the case of a gang programmer being used, this function does
                    not verify whether the chips can be communicated with.

*************************************************************************************/
TESTFLASH_API(int32) flOpenUnlockTrans(const char* trans, int32 xtal,
                                       int32 delays, const char* unlockKey);

/*************************************************************************************

    Function :      int32 flmOpenTrans(uint32 deviceMask, const char* trans,
                                       int32 xtal);

    Parameters :    deviceMask -
                        See flmOpen for the details of this parameter.

                    trans -
                        See flOpenTrans. The transport string given to flmOpenTrans
                        cannot contain a SPIPORT specifier (see
                        flmGetAvailablePorts and flmConvertPort).

                    xtal -
                        See flOpenTrans.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This is the multiple connection version of flOpenTrans. See
                    flOpenTrans for more details. flmOpen can also be used as an
                    alternative to flmOpenTrans.
                    <p>
                    To be used (only) with devices which are not transport locked.
                    <p>
                    flmOpenUnlock or flmOpenUnlockTrans should be used with transport
                    locked devices.
                    <p>
                    This function will fail if the chip(s) cannot be communicated
                    with. If the return value is TFL_DEVICE_ERROR, use
                    flmGetBitErrorField to determine which devices failed / succeeded.
                    <p>
                    This is the entry point for the multiple connection version of
                    TestFlash. This must be used when programming multiple devices,
                    but not when using a parallel port gang programmer to do so.
                    <p>
                    flmOpenTrans can either be called with a mask specifying all
                    devices to open, or it can be called multiple times to open
                    multiple devices (the devices in the mask must be different for
                    each call). In the case of multiple calls to flmOpen, the
                    transport type must be the same for each call.

*************************************************************************************/
TESTFLASH_API(int32) flmOpenTrans(uint32 deviceMask, const char* trans, int32 xtal);

/*************************************************************************************

    Function :      int32 flmOpenUnlockTrans(uint32 deviceMask, const char* trans,
                                             int32 xtal, const char* unlockKey);

    Parameters :    deviceMask -
                        See flmOpen for the details of this parameter.

                    trans -
                        See flOpenTrans. The transport string given to flmOpenTrans
                        cannot contain a SPIPORT specifier (see
                        flmGetAvailablePorts and flmConvertPort).

                    xtal -
                        See flOpenTrans.

                    unlockKey -
                        Defines the unlock key. By default, the unlock key should be
                        supplied unencrypted. For USBDBG only, the user can override
                        the default and supply an encrypted key - this must be
                        prefaced with 'ENC', for example,
                        "ENCdc0ed85df9611abb7249cdd168c5467e".

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This is the multiple connection version of flOpenUnlockTrans. See
                    flOpenUnlockTrans for more details.
                    flmOpenUnlock can also be used as an alternative to
                    flmOpenUnlockTrans.
                    <p>
                    To be used (only) with transport locked devices.
                    <p>
                    flmOpen or flmOpenTrans should be used with devices which are not
                    transport locked.
                    <p>
                    This function will fail if the chip(s) cannot be communicated
                    with. If the return value is TFL_DEVICE_ERROR, use
                    flmGetBitErrorField to determine which devices failed / succeeded.
                    <p>
                    This is the entry point for the multiple connection version of
                    TestFlash. This must be used when programming multiple devices,
                    but not when using a parallel port gang programmer to do so.
                    <p>
                    flmOpenTrans can either be called with a mask specifying all
                    devices to open, or it can be called multiple times to open
                    multiple devices (the devices in the mask must be different for
                    each call). In the case of multiple calls to flmOpen, the
                    transport type must be the same for each call.

 *************************************************************************************/
TESTFLASH_API(int32) flmOpenUnlockTrans(uint32 deviceMask, const char* trans,
                                        int32 xtal, const char* unlockKey);

/*************************************************************************************

    Function :      int32 flReadProgramFiles(const char* fileName)

    Parameters :    fileName -
                        String containing the image file path file path, where the
                        supported file types (identified by their extension) are:
                        <ol>
                            <li>XUV
                            <li>XPV/XDV
                            <li>IMG
                        </ol>
                        If the given file extension is XPV or XDV, both XPV and XDV
                        extension files will be read.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is used to load image files into memory.

*************************************************************************************/
TESTFLASH_API(int32) flReadProgramFiles(const char* fileName);

/*************************************************************************************

    Function :      int32 flmReadProgramFiles(const char* fileName)

    Parameters :    fileName -
                        See flReadProgramFiles. If this parameter is a zero-length
                        string then the current image is cleared and TFL_OK is
                        returned.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This is the multiple connection version of flReadProgramFiles.
                    See flReadProgramFiles for more information. flmReadProgramFiles
                    can be called before flmOpen and flmOpenTrans.

*************************************************************************************/
TESTFLASH_API(int32) flmReadProgramFiles(const char* fileName);

/*************************************************************************************

    Function :      int32 flProgramBlock(void)

    Parameters :    None

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_DOWNLOAD_FAILED             <td> 1
                            <tr><td>FLASH_ERROR_VERIFY_FAILED               <td> 2
                            <tr><td>FLASH_ERROR_TIMED_OUT                   <td> 3
                            <tr><td>FLASH_ERROR_CRC_FAILED                  <td> 4
                            <tr><td>FLASH_ERROR_READBACK_FAILED             <td> 5
                            <tr><td>FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG     <td> 6
                            <tr><td>FLASH_ERROR_COULD_NOT_STOP_XAP          <td> 7
                            <tr><td>FLASH_ERROR_BOOT_PROG_HALTED            <td> 8
                            <tr><td>FLASH_ERROR_ERASE_FAILED                <td> 9
                            <tr><td>FLASH_ERROR_XAP_ERROR                   <td> 10
                            <tr><td>FLASH_ERROR_UNKNOWN_CHIP_TYPE           <td> 11
                            <tr><td>FLASH_ERROR_BROADCAST_MIXED_CHIP_TYPES  <td> 12
                            <tr><td>FLASH_ERROR_OPEN_FILE                   <td> 13
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                            <tr><td>FLASH_ERROR_NO_FLASH                    <td> 16
                            <tr><td>FLASH_ERROR_OOM (out of memory)         <td> 17
                        </table>

    Description :   This function is used to download the image data to the device.
                    The function will block until the process has completed
                    successfully or an error occurs.

*************************************************************************************/
TESTFLASH_API(int32) flProgramBlock(void);

/*************************************************************************************

    Function :      int32 flmProgramBlock(uint32 deviceMask, uint8 eraseFirst,
                                          uint8 verifyAfter, uint8 restartAfter)

    Parameters :    deviceMask -
                        Specifies which devices will be programmed. See flmOpen for
                        the details of this parameter.

                    eraseFirst -
                        Set to 1 to erase the entire NVM of all devices before
                        programming. Set to 0 if erase is not required.

                    verifyAfter -
                        Set to 1 to verify the NVM contents of devices after
                        completion of the programming process. Set to 0 if verify is
                        not required.

                    restartAfter -
                        Set to 1 to restart the device after completion of the
                        programming process. Set to 0 if restart is not required.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This function is used to download the image to up to 32 devices
                    over multiple ports. This function will block execution until all
                    devices have been programmed successfully or have failed.
                    <p>
                    If this function returns TFL_DEVICE_ERROR, use flmGetBitErrorField
                    to determine which devices failed.

*************************************************************************************/
TESTFLASH_API(int32) flmProgramBlock(uint32 deviceMask, uint8 eraseFirst,
                                     uint8 verifyAfter, uint8 restartAfter);

/*************************************************************************************

    Function :      int32 flErase(void)

    Parameters :    None

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_DOWNLOAD_FAILED             <td> 1
                            <tr><td>FLASH_ERROR_VERIFY_FAILED               <td> 2
                            <tr><td>FLASH_ERROR_TIMED_OUT                   <td> 3
                            <tr><td>FLASH_ERROR_CRC_FAILED                  <td> 4
                            <tr><td>FLASH_ERROR_READBACK_FAILED             <td> 5
                            <tr><td>FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG     <td> 6
                            <tr><td>FLASH_ERROR_COULD_NOT_STOP_XAP          <td> 7
                            <tr><td>FLASH_ERROR_BOOT_PROG_HALTED            <td> 8
                            <tr><td>FLASH_ERROR_ERASE_FAILED                <td> 9
                            <tr><td>FLASH_ERROR_XAP_ERROR                   <td> 10
                            <tr><td>FLASH_ERROR_UNKNOWN_CHIP_TYPE           <td> 11
                            <tr><td>FLASH_ERROR_BROADCAST_MIXED_CHIP_TYPES  <td> 12
                            <tr><td>FLASH_ERROR_OPEN_FILE                   <td> 13
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                            <tr><td>FLASH_ERROR_NO_FLASH                    <td> 16
                            <tr><td>FLASH_ERROR_OOM (out of memory)         <td> 17
                        </table>

    Description :   This function is used to erase the NVM.

*************************************************************************************/
TESTFLASH_API(int32) flErase(void);

/*************************************************************************************

    Function :      int32 flmEraseBlock(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be erased. See flmOpen for the
                        details of this parameter.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This function is used to erase the NVM of to up to 32 devices
                    over multiple ports. This function will block execution until all
                    devices have been erased successfully or an error occurs.
                    <p>
                    If this function returns TFL_DEVICE_ERROR, use flmGetBitErrorField
                    to determine which devices failed.

*************************************************************************************/
TESTFLASH_API(int32) flmEraseBlock(uint32 deviceMask);

/*************************************************************************************

    Function :      int32 flmEraseSpawn(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be erased. See flmOpen for the
                        details of this parameter.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is used to erase the NVM of to up to 32 devices
                    over multiple ports. This function will spawn the threads which
                    perform the erase.
                    <p>
                    NOTE: The return value indicates only whether the erase threads
                    started successfully or not. When the erase completes,
                    flmGetDeviceError should be called to get the final status.
                    flmGetDeviceProgress should be used to check for completion.

*************************************************************************************/
TESTFLASH_API(int32) flmEraseSpawn(uint32 deviceMask);

/*************************************************************************************

    Function :      int32 flGangProgramBlock(uint16 deviceMask, uint8 eraseFirst,
                                             uint8 restartAfter, uint8 skipUnused)

    Parameters :    deviceMask -
                        Specifies which devices will be programmed. Uses a bit-field
                        so 5 = 0000000000000101 = devices 0 and 2. A value of 0 (zero)
                        will auto-detect and program all devices present.

                    eraseFirst -
                        Set to 1 to erase the entire NVM of all devices before
                        programming. Set to 0 if erase is not required.

                    restartAfter -
                        Set to 1 to restart all devices after completion of the
                        programming process. Set to 0 if restart is not required.

                    skipUnused -
                        Set to 1 to skip attempts to stop and restart devices at
                        unused positions. Set to zero to attempt to stop and restart
                        all positions regardless of deviceMask value (this takes some
                        time).
                        <p>
                        NOTE: skipUnused has no effect when deviceMask = 0
                        (auto-detect).

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_DOWNLOAD_FAILED             <td> 1
                            <tr><td>FLASH_ERROR_VERIFY_FAILED               <td> 2
                            <tr><td>FLASH_ERROR_TIMED_OUT                   <td> 3
                            <tr><td>FLASH_ERROR_CRC_FAILED                  <td> 4
                            <tr><td>FLASH_ERROR_READBACK_FAILED             <td> 5
                            <tr><td>FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG     <td> 6
                            <tr><td>FLASH_ERROR_COULD_NOT_STOP_XAP          <td> 7
                            <tr><td>FLASH_ERROR_BOOT_PROG_HALTED            <td> 8
                            <tr><td>FLASH_ERROR_ERASE_FAILED                <td> 9
                            <tr><td>FLASH_ERROR_XAP_ERROR                   <td> 10
                            <tr><td>FLASH_ERROR_UNKNOWN_CHIP_TYPE           <td> 11
                            <tr><td>FLASH_ERROR_BROADCAST_MIXED_CHIP_TYPES  <td> 12
                            <tr><td>FLASH_ERROR_OPEN_FILE                   <td> 13
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                            <tr><td>FLASH_ERROR_NO_FLASH                    <td> 16
                            <tr><td>FLASH_ERROR_OOM (out of memory)         <td> 17
                        </table>

                    If all the specified devices were programmed correctly the function
                    will return FLASH_ERROR_NONE, if there were 1 or more programming
                    failures then the function will return
                    FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG. All other error returns denote
                    a gross failure.

    Description :   This function is used to download the image to up to 16 devices
                    when using the gang programmer reference device. This function
                    will block execution until the process completes successfully or
                    an error occurs.
                    <p>
                    On completion with an error return of either FLASH_ERROR_NONE or
                    FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG the programmer should call
                    flGetBitErrorField to determine which device(s) failed to program.

*************************************************************************************/
TESTFLASH_API(int32) flGangProgramBlock(uint16 deviceMask, uint8 eraseFirst,
                                        uint8 restartAfter, uint8 skipUnused);

/*************************************************************************************

    Function :      uint16 flGetDetectedDevices(void)

    Parameters :    None

    Returns :       Bitfield indicating which devices were detected when using
                    auto-detect mode with gang programmer hardware. A value of 0 means
                    that no devices were detected. A value of 5 indicates that devices
                    0 and 2 were detected.

    Description :   This function can be used after gang programming operations in
                    auto-detect mode to determine which devices were detected. It can
                    be used with flGetBitErrorField to determine which devices were
                    successfully programmed (those devices which were detected and
                    didn't fail).
                    <p>
                    auto-detect mode is used when the device_mask parameter given to
                    flGangProgramBlock or flGangProgramSpawn is 0.

*************************************************************************************/
TESTFLASH_API(uint16) flGetDetectedDevices(void);

/*************************************************************************************

    Function :      uint16 flGetBitErrorField(void)

    Parameters :    None

    Returns :       Bitfield referring to the devices present on the gang programmer
                    and those that have failed. Will return
                    (failed_devices & deviceMask).
                    Therefore a return of zero (0) means all devices have been
                    programmed successfully. A return value of five (5) denotes that
                    devices 0 and 2 have failed and the remainder have successfully
                    been programmed.
                    <p>
                    If the last broadcast operation was performed in Auto-detect mode
                    and no devices were detected, then flGetBitErrorField will return
                    the value 0xFFFF.

    Description :   This function is used to determine which devices have failed to be
                    programmed during the gang programming process.

*************************************************************************************/
TESTFLASH_API(uint16) flGetBitErrorField(void);

/*************************************************************************************

    Function :      uint32 flmGetBitErrorField(void)

    Parameters :    None

    Returns :       Bitfield indicating which devices have failed. A return value of
                    0 means that the previous operation completed successfully on all
                    devices. A return value of 5 denotes that devices 0 and 2 have
                    failed and any other devices completed without errors.

    Description :   This function is used to determine which devices failed during the
                    last multiple connection operation. flmGetBitErrorField is valid
                    after a function returns TFL_DEVICE_ERROR or after a spawned
                    process indicates completion (flmGetDeviceProgress returns 100
                    for all devices).

*************************************************************************************/
TESTFLASH_API(uint32) flmGetBitErrorField(void);

/*************************************************************************************

    Function :      int32 flProgramSpawn(void)

    Parameters :    None

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                        </table>

    Description :   This function is used to download the image to the device under
                    test. The function will spawn the thread which performs the NVM
                    programming.
                    <p>
                    NOTE: The return value indicates only whether the programming
                    thread started successfully or not. When programming completes,
                    flGetLastError should be called to get the final status.
                    flGetProgress should be used to check for completion.

*************************************************************************************/
TESTFLASH_API(int32) flProgramSpawn(void);

/*************************************************************************************

    Function :      int32 flmProgramSpawn(uint32 deviceMask, uint8 eraseFirst,
                                          uint8 verifyAfter, uint8 restartAfter)

    Parameters :    deviceMask -
                        Specifies which devices will be programmed. See flmOpen for
                        the details of this parameter.

                    eraseFirst -
                        Set to 1 to erase the entire NVM of all devices before
                        programming. Set to 0 if erase is not required.

                    verifyAfter -
                        Set to 1 to verify the NVM contents of devices after
                        completion of the programming process. Set to 0 if verify is
                        not required.

                    restartAfter -
                        Set to 1 to restart all devices after completion of the
                        programming process. Set to 0 if restart is not required.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is used to download the image to up to 32 devices
                    over multiple connections. This function will spawn the threads
                    which perform the NVM programming.
                    <p>
                    NOTE: The return value indicates only whether the programming
                    threads started successfully or not. When programming completes,
                    flmGetDeviceError should be called to get the final status.
                    flmGetDeviceProgress should be used to check for completion.

*************************************************************************************/
TESTFLASH_API(int32) flmProgramSpawn(uint32 deviceMask, uint8 eraseFirst,
                                     uint8 verifyAfter, uint8 restartAfter);

/*************************************************************************************

    Function :      int32 flGangProgramSpawn(uint16 deviceMask, uint8 eraseFirst,
                                             uint8 restartAfter, uint8 skipUnused)


    Parameters :    deviceMask -
                        Specifies which devices will be programmed. Uses a bit-field
                        so 5 = 0000000000000101 = devices 0 and 2. A value of 0 (zero)
                        will auto-detect and program all devices present.

                    eraseFirst -
                        Set to 1 to erase the entire NVM of all devices before
                        programming. Set to 0 if erase is not required.

                    restartAfter -
                        Set to 1 to restart the XAP processors of all devices after
                        completion of the programming process. Set to 0 if restart is
                        not required.

                    skipUnused -
                        Set to 1 to skip attempts to stop and restart devices at
                        unused positions. Set to zero to attempt to stop and restart
                        all positions regardless of deviceMask value (this takes
                        some time).
                        <p>
                        NOTE: skipUnused has no effect when deviceMask = 0
                        (auto-detect).

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                        </table>

    Description :   This function is used to download the image to one or more
                    devices connected to the gang programmer hardware. The function
                    will spawn the threads which perform the NVM programming.
                    <p>
                    NOTE: The return value indicates only whether the programming
                    threads started successfully or not. When programming completes,
                    flGetLastError should be called to get the final status.
                    flGetBitErrorField should then be called to determine which, if
                    any, of the devices failed. flGetProgress can be used to check
                    for completion.

*************************************************************************************/
TESTFLASH_API(int32) flGangProgramSpawn(uint16 deviceMask, uint8 eraseFirst,
                                        uint8 restartAfter, uint8 skipUnused);

/*************************************************************************************

    Function :      int32 flGetProgress(void)

    Parameters :    None

    Returns :       Progress as a percentage (0 to 100). 100 indicates that the
                    spawned programming thread has returned with or without an error.

    Description :   This function is used to check the progress of the programming
                    thread spawned by flProgramSpawn or flGangProgramSpawn.

*************************************************************************************/
TESTFLASH_API(int32) flGetProgress(void);

/*************************************************************************************

    Function :      int32 flmGetDeviceProgress(uint32 device)

    Parameters :    device -
                        The index of the device for which to retrieve the progress.
                        E.g. to retrieve the progress of the 3rd device, the parameter
                        is 2. For LPT-SPI transports only, device indices start at 1,
                        so the parameter value for the 3rd device would be 3 in this
                        case.

    Returns :       Progress as a percentage (0 to 100). 100 indicates that the
                    spawned programming thread has returned with or without an error.

    Description :   This function is used to check the progress of an operation
                    spawned by flmProgramSpawn, flmEraseSpawn and flmVerifySpawn.

*************************************************************************************/
TESTFLASH_API(int32) flmGetDeviceProgress(uint32 device);

/*************************************************************************************

    Function :      int32 flGetLastError(void)

    Parameters :    None

    Returns :       One of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_DOWNLOAD_FAILED             <td> 1
                            <tr><td>FLASH_ERROR_VERIFY_FAILED               <td> 2
                            <tr><td>FLASH_ERROR_TIMED_OUT                   <td> 3
                            <tr><td>FLASH_ERROR_CRC_FAILED                  <td> 4
                            <tr><td>FLASH_ERROR_READBACK_FAILED             <td> 5
                            <tr><td>FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG     <td> 6
                            <tr><td>FLASH_ERROR_COULD_NOT_STOP_XAP          <td> 7
                            <tr><td>FLASH_ERROR_BOOT_PROG_HALTED            <td> 8
                            <tr><td>FLASH_ERROR_ERASE_FAILED                <td> 9
                            <tr><td>FLASH_ERROR_XAP_ERROR                   <td> 10
                            <tr><td>FLASH_ERROR_UNKNOWN_CHIP_TYPE           <td> 11
                            <tr><td>FLASH_ERROR_BROADCAST_MIXED_CHIP_TYPES  <td> 12
                            <tr><td>FLASH_ERROR_OPEN_FILE                   <td> 13
                            <tr><td>FLASH_ERROR_NO_IMAGE                    <td> 14
                            <tr><td>FLASH_ERROR_BUSY                        <td> 15
                            <tr><td>FLASH_ERROR_NO_FLASH                    <td> 16
                            <tr><td>FLASH_ERROR_OOM (out of memory)         <td> 17
                            <tr><td>TFL_ERROR_OPEN_FAILED                   <td> 4097
                            <tr><td>TFL_ERROR_DEVICE_OPEN                   <td> 4098
                            <tr><td>TFL_ERROR_DEVICE_NOT_OPEN               <td> 4099
                            <tr><td>TFL_ERROR_DEVICE_BUSY                   <td> 4100
                            <tr><td>TFL_ERROR_THREAD_ERROR                  <td> 4101
                            <tr><td>TFL_ERROR_RESET_FAIL                    <td> 4102
                            <tr><td>TFL_ERROR_XTAL_INVALID                  <td> 4103
                        </table>

    Description :   This function will return the last error generated by the NVM
                    programming stack. If called after flGetProgress returns 100 it
                    can be used to check for programming errors after spawned
                    operations.

*************************************************************************************/
TESTFLASH_API(int32) flGetLastError(void);

/*************************************************************************************

    Function :      int32 flmGetLastError(void)

    Parameters :    None

    Returns :       See flGetLastError.

    Description :   This returns the error that caused the last multiple connection
                    function called to return TFL_ERROR. This function does not
                    return errors encountered by specific devices, such as when a
                    function returns TFL_DEVICE_ERROR; Use flmGetDeviceError to
                    retrieve those errors.

*************************************************************************************/
TESTFLASH_API(int32) flmGetLastError(void);

/*************************************************************************************

    Function :      int32 flmGetDeviceError(uint32 device)

    Parameters :    device -
                        The index of the device for which to retrieve the error code.
                        E.g. to retrieve the error code for the 3rd device, the
                        parameter is 2. For LPT-SPI transports only, device indices
                        start at 1, so the parameter value for the 3rd device would be
                        3 in this case.

    Returns :       See flGetLastError.

    Description :   This function returns the error status for a specific device.
                    This function should be used after flmOpen, flmEraseBlock,
                    flmProgramBlock or flmVerifyBlock returns TFL_DEVICE_ERROR, or
                    when a spawned operation completes and flmGetBitErrorField shows
                    an error for a device.

*************************************************************************************/
TESTFLASH_API(int32) flmGetDeviceError(uint32 device);

/*************************************************************************************

    Function :      int32 flResetAndStart(void)

    Parameters :    None

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function can be used to reset and start the device.
                    <p>
                    The current connection to the device is considered to be invalid
                    after this call, and it must be closed (see flClose
                    / flCloseRet). To open a new connection to the device use
                    flOpenTrans (or flOpenUnlockTrans if the Debug Lock is enabled).

*************************************************************************************/
TESTFLASH_API(int32) flResetAndStart(void);

/*************************************************************************************

    Function :      int32 flmResetAndStart(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be reset and started. See flmOpen
                        for the details of this parameter.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This is the multiple connection version of flResetAndStart. If
                    this function returns TFL_DEVICE_ERROR, use flmGetBitErrorField
                    to determine which devices failed.
                    <p>
                    The current connection to the device is considered to be invalid
                    after this call, and it must be closed (see flmClose
                    / flmCloseRet). To open a new connection to the device use
                    flmOpenTrans (or flmOpenUnlockTrans if the Debug Lock is enabled).

*************************************************************************************/
TESTFLASH_API(int32) flmResetAndStart(uint32 deviceMask);

/*************************************************************************************

    Function :      void flClose(void)

    Parameters :    None

    Returns :       None

    Description :   Performs shut-down activities (such as relocking the transport, if
                    applicable) for any device connection, before disconnecting the
                    device connection and freeing any memory used by TestFlash.
                    Transport relocking is only supported on certain devices.
                    <p>
                    Note that if the connection was opened using flOpenUnlock, it is
                    recommended to close the connection using flCloseRet, as
                    flCloseRet will indicate by its return code if relocking the
                    transport failed.

*************************************************************************************/
TESTFLASH_API(void) flClose(void);

/*************************************************************************************

    Function :      int32 flCloseRet(void)

    Parameters :    None

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Performs shut-down activities (such as relocking the transport, if
                    applicable) for any device connection, before disconnecting the
                    device connection and freeing any memory used by TestFlash.
                    Transport relocking is only supported on certain devices.
                    <p>
                    A return code of TFL_ERROR indicates a problem in the shut-down
                    activities, e.g. relocking the transport failed (in a scenario
                    where flOpenUnlock was used to open the connection).

*************************************************************************************/
TESTFLASH_API(int32) flCloseRet(void);

/*************************************************************************************

    Function :      void flmClose(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be closed. See flmOpen for the
                        details of this parameter.

    Returns :       None

    Description :   This is the multiple connection version of flClose. Refer to
                    flClose for more details.

*************************************************************************************/
TESTFLASH_API(void) flmClose(uint32 deviceMask);

/*************************************************************************************

    Function :      int32 flmCloseRet(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be closed. See flmOpen for the
                        details of this parameter.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This is the multiple connection version of flCloseRet. Refer to
                    flCloseRet for more details.

*************************************************************************************/
TESTFLASH_API(int32) flmCloseRet(uint32 deviceMask);

/*************************************************************************************

    Function :      int32 flGetVersion(char* versionStr)

    Parameters :    versionStr -
                        Pre-allocated string for receiving the version number.

    Returns :       Always returns TFL_OK (0).

    Description :   Gets the version string for this DLL.

*************************************************************************************/
TESTFLASH_API(int32) flGetVersion(char* versionStr);

/*************************************************************************************

    Function :      int32 flmGetVersion(char* versionStr)

    Parameters :    versionStr -
                        See flGetVersion.

    Returns :       See flGetVersion.

    Description :   This is a synonym for flGetVersion.

*************************************************************************************/
TESTFLASH_API(int32) flmGetVersion(char* versionStr);

/*************************************************************************************

    Function :      int32 flStopProcessor(void)

    Parameters :    None

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function will stop the processor of the connected device.
                    <p>
                    A SPI connection has to be made for this function to complete
                    successfully.
                    <p>
                    This function is only applicable for BlueCore and Bluetooth low
                    energy ICs.

*************************************************************************************/
TESTFLASH_API(int32) flStopProcessor(void);

/*************************************************************************************

    Function :      int32 flmStopProcessor(uint32 deviceMask)

    Parameters :    deviceMask -
                        Specifies which devices will be stopped. See flmOpen for the
                        details of this parameter.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This is the multiple connection version of flStopProcessor. Refer
                    to flStopProcessor for more details. If this function returns
                    TFL_DEVICE_ERROR, use flmGetBitErrorField to determine which
                    devices failed.
                    <p>
                    This function is only applicable for BlueCore and Bluetooth low
                    energy ICs.

*************************************************************************************/
TESTFLASH_API(int32) flmStopProcessor(uint32 deviceMask);

/*************************************************************************************

    Function :      uint32 flGetDownloadTime(void)

    Parameters :    None

    Returns :       The time taken in milliseconds for the last download. Returns 0
                    if an error occurred or if download has not completed.

    Description :   Fetches the last download time.

*************************************************************************************/
TESTFLASH_API(uint32) flGetDownloadTime(void);

/*************************************************************************************

    Function :      uint32 flmGetDeviceDownloadTime(uint32 device)

    Parameters :    device -
                        The index of the device for which to retrieve the download
                        time. E.g. to retrieve the time for the 3rd device, the
                        parameter is 2. For LPT-SPI transports only, device indices
                        start at 1, so the parameter value for the 3rd device would be
                        3 in this case.

    Returns :       See flGetDownloadTime.

    Description :   This is the multiple connection version of flGetDownloadTime.
                    Refer to flGetDownloadTime for more details.

*************************************************************************************/
TESTFLASH_API(uint32) flmGetDeviceDownloadTime(uint32 device);

/*************************************************************************************

    Function :      int32 flGetAvailablePorts(uint16* maxLen, char* ports,
                                              char* trans, uint16* count)

    Parameters :    maxLen -
                        Size of the arrays pointed to by the ports and trans
                        parameters. If this parameter indicates that the ports or
                        trans arrays are too small to store the complete strings,
                        then the value is set to the size required (and error is
                        returned).
                        If any other error occurs, this value is set to zero.

                    ports -
                        Pointer to an array of ASCII chars where the comma separated
                        list of available debug port names will be stored. These are
                        readable names which could be used for user selection.

                    trans -
                        Pointer to an array of ASCII chars where the comma separated
                        list of debug transport options for each of the available
                        ports will be stored. The transport options for a port can be
                        passed directly into the flOpenTrans function to open the
                        port.

                    count -
                        This value is set to the number of available ports found.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function is used to get a list of available debug ports. A
                    char array, pointed to by the ports parameter, is filled with a
                    comma separated list of port names. A further char array, pointed
                    to by the trans parameter, is filled with a comma separated list
                    of the relevant transport option strings that specify each
                    available debug port.
                    <p>
                    If the maxLen parameter indicates that either char array is not
                    large enough to contain the strings, Error is returned and the
                    maxLen parameter is set to the size required for the arrays.
                    <p>
                    If any other error occurs, the maxLen parameter is set to 0.
                    <p>
                    This function can be used by an application to get a list of
                    available debug ports with which to populate a drop down list or
                    other means of selection. The strings returned in the ports
                    parameter are in human readable format for display / selection.
                    The strings returned in the trans parameter can be passed directly
                    to the flOpenTrans function to open the port.

    Example :

        uint16 maxLen(256);
        uint16 count(0);
        char* portsStr = new char[maxLen];
        char* transStr = new char[maxLen];
        vector<string> ports; // The human readable port strings (e.g. "USBDBG(100)")
        vector<string> trans; // The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")

        int32 status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
        if( status != TFL_OK && maxLen != 0 )
        {
            // Not enough space - resize the storage
            delete[] portsStr;
            portsStr = new char[maxLen];
            delete[] transStr;
            transStr = new char[maxLen];
            status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
        }
        if( status != TFL_OK || count == 0 )
        {
            cout << "Error getting ports, or none found" << endl;
            delete[] portsStr;
            delete[] transStr;
            return;
        }

        // Split up the comma separated strings of ports / transport options
        split(ports, portsStr, ','); // Use these for user selection (e.g. drop down list)
        split(trans, transStr, ','); // Use these to open a transport

        // Open the port using the trans string
        // For the purposes of this example, we're just using the first in the list
        status = flOpenTrans(trans.at(0).c_str(), 0, 0);

        if(status == TFL_OK)
        {
            cout << "Connected!" << endl;

            // Download code here

            flClose();
        }

        delete[] portsStr;
        delete[] transStr;

        return;

*************************************************************************************/
TESTFLASH_API(int32) flGetAvailablePorts(uint16* maxLen, char* ports, char* trans,
                                         uint16* count);

/*************************************************************************************

    Function :      int32 flmGetAvailablePorts(uint16* maxLen, char* ports,
                                               char* trans, uint16* count)

    Parameters :    maxLen -
                        See flGetAvailablePorts.

                    ports -
                        See flGetAvailablePorts.

                    trans -
                        See flGetAvailablePorts.

                    count -
                        See flGetAvailablePorts.

    Returns :       See flGetAvailablePorts.

    Description :   This is the multiple connection version of flGetAvailablePorts.
                    Refer to flGetAvailablePorts for more details.
                    <p>
                    NOTE: While the functionality is similar to
                    flGetAvailablePorts, the transport strings returned in the
                    trans parameter cannot be passed directly to flmOpenTrans.
                    flmConvertPort should be used to convert the string into the
                    appropriate string and device mask, both of which can be passed
                    to flmOpenTrans.

*************************************************************************************/
TESTFLASH_API(int32) flmGetAvailablePorts(uint16* maxLen, char* ports, char* trans,
                                          uint16* count);

/*************************************************************************************
    Function :      int32 flmConvertPort(const char* transIn, char* transOut,
                                         uint32* device)

    Parameters :    transIn -
                        String of space separated transport options.

                    transOut -
                        Pre-allocated string for receiving the altered transport
                        string. Must be at least the same length as transIn.

                    device -
                        Location for receiving the device identifier extracted from
                        the transport string. This is a port number for the device,
                        not a mask, but can be bit-shifted to create a mask as shown
                        in the example.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This function will convert a transport string returned by
                    flmGetAvailablePorts into a transport string and device
                    identifier. The resulting device mask and the converted string
                    can be used with flmOpenTrans (The SPIPORT specifier is removed
                    from the string).

    Example :

        uint16 maxLen(256);
        uint16 count(0);
        char* portsStr = new char[maxLen];
        char* transStr = new char[maxLen];
        vector<string> ports; // The human readable port strings (e.g. "USBDBG(100)")
        vector<string> trans; // The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")

        int32 status = flmGetAvailablePorts(&maxLen, portsStr, transStr, &count);
        if( status != TFL_OK && maxLen != 0 )
        {
            // Not enough space - resize the storage
            delete[] portsStr;
            portsStr = new char[maxLen];
            delete[] transStr;
            transStr = new char[maxLen];
            status = flmGetAvailablePorts(&maxLen, portsStr, transStr, &count);
        }
        if( status != TFL_OK || count == 0 )
        {
            cout << "Error getting ports, or none found" << endl;
            delete[] portsStr;
            delete[] transStr;
            return;
        }

        // Split up the comma separated strings of ports / transport options
        split(ports, portsStr, ','); // Use these for user selection (e.g. drop down list)
        split(trans, transStr, ','); // Use these to open a transport

        // Convert the port specification in to a specification suitable for multiple
        // device open
        // For the purposes of this example, we're just using the first in the list
        char* newTransStr = new char[trans.at(0).length() + 1];
        uint32 device;
        status = flmConvertPort(trans.at(0).c_str(), newTransStr, &device);
        if( status != TFL_OK )
        {
            cout << "Error converting port to transport" << endl;
            delete[] newTransStr;
            delete[] portsStr;
            delete[] transStr;
            return;
        }

        // Open the port using the trans string
        status = flmOpenTrans(1 << device, newTransStr, 26);

        if(status == TFL_OK)
        {
            cout << "Connected!" << endl;

            // Download code here

            flmClose(1 << device);
        }

        delete[] newTransStr;
        delete[] portsStr;
        delete[] transStr;

        return;

*************************************************************************************/
TESTFLASH_API(int32) flmConvertPort(const char* transIn, char* transOut,
                                    uint32* device);

/*************************************************************************************

    Function :      int32 flGetFirmwareIds(uint8 source, uint32* loaderId,
                                           char* loaderName, uint32* stackId,
                                           char* stackName, uint16 maxNameLen)

    Parameters :    source -
                        Value indicating where the firmware id information should be
                        read from, where:
                        <table>
                            <tr><td>TFL_CHIP <td>0 <td>(Read from attached chip)
                            <tr><td>TFL_FILE <td>1 <td>(Read from file)
                        </table>

                    loaderId -
                        Location for receiving the loader id value.

                    loaderName -
                        Pre-allocated string for receiving the loader name.

                    stackId -
                        Location for receiving the stack id value.

                    stackName -
                        Pre-allocated string for receiving the stack name.

                    maxNameLen -
                        The length of the buffers used for the loaderName and
                        stackName strings.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Gets the firmware id information from an attached device or from
                    a pre-loaded file (loaded using flReadProgramFiles).
                    <p>
                    This function is only applicable to BlueCore ICs.

*************************************************************************************/
TESTFLASH_API(int32) flGetFirmwareIds(uint8 source, uint32* loaderId,
                                      char* loaderName, uint32* stackId,
                                      char* stackName, uint16 maxNameLen);

/*************************************************************************************

    Function :      int32 flmGetDeviceFirmwareIds(uint32 device,
                                                  uint32* loaderId, char* loaderName,
                                                  uint32* stackId, char* stackName,
                                                  uint16 maxNameLen)

    Parameters :    device -
                        The index of the device for which to retrieve the firmware
                        IDs. E.g. to retrieve the firmware IDs for the 3rd device, the
                        parameter is 2. For LPT-SPI transports only, device indices
                        start at 1, so the parameter value for the 3rd device would be
                        3 in this case.

                    loaderId -
                        See flGetFirmwareIds.

                    loaderName -
                        See flGetFirmwareIds.

                    stackId -
                        See flGetFirmwareIds.

                    stackName -
                        See flGetFirmwareIds.

                    maxNameLen -
                        See flGetFirmwareIds.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This is the multiple connection version of flGetFirmwareIds.
                    Refer to flGetFirmwareIds for more details. This function always
                    returns the firmware id of the chip.
                    <p>
                    This function is only applicable to BlueCore ICs.

*************************************************************************************/
TESTFLASH_API(int32) flmGetDeviceFirmwareIds(uint32 device,
                                             uint32* loaderId, char* loaderName,
                                             uint32* stackId, char* stackName,
                                             uint16 maxNameLen);

/*************************************************************************************

    Function :      int32 flmGetFileFirmwareIds(uint32* loaderId, char* loaderName,
                                                uint32* stackId, char* stackName,
                                                uint16 maxNameLen)

    Parameters :    loaderId -
                        See flGetFirmwareIds.

                    loaderName -
                        See flGetFirmwareIds.

                    stackId -
                        See flGetFirmwareIds.

                    stackName -
                        See flGetFirmwareIds.

                    maxNameLen -
                        See flGetFirmwareIds.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Refer to flGetFirmwareIds for more details. This function always
                    returns the firmware id of the current file (not from the chip).
                    <p>
                    This function is only applicable for BlueCore ICs.

*************************************************************************************/
TESTFLASH_API(int32) flmGetFileFirmwareIds(uint32* loaderId, char* loaderName,
                                           uint32* stackId, char* stackName,
                                           uint16 maxNameLen);

/*************************************************************************************

    Function :      int32 flVerify(void)

    Parameters :    None

    Returns :       TFL_ERROR (-1) if there is a general error within TestFlash
                    (normally due to an incorrect sequence of API calls), otherwise
                    one of the codes as follows:
                        <table>
                            <tr><td>FLASH_ERROR_NONE                        <td> 0
                            <tr><td>FLASH_ERROR_VERIFY_FAILED               <td> 2
                        </table>

                    <p>It is possible for other error codes (values > 0) to be
                    returned, e.g. if there are transport problems. These error codes
                    will be from the set listed for flGetLastError.

    Description :   This function is used to verify the contents of NVM against the
                    file loaded using flReadProgramFiles. Therefore,
                    flReadProgramFiles must be called prior to calling this function.

*************************************************************************************/
TESTFLASH_API(int32) flVerify(void);

/*************************************************************************************

    Function :      int32 flmVerifyBlock(uint32 deviceMask, uint8 restartAfter)

    Parameters :    deviceMask -
                        Specifies which devices will be verified. See flmOpen for the
                        details of this parameter.

                    restartAfter -
                        Set to 1 to restart all devices after completion of the verify
                        process. Set to 0 if restart is not required.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                        <tr><td>TFL_DEVICE_ERROR    <td>-2
                    </table>

    Description :   This is the multiple connection version of flVerify. Refer to
                    flVerify for more details. This function blocks until all devices
                    have verified or failed.

*************************************************************************************/
TESTFLASH_API(int32) flmVerifyBlock(uint32 deviceMask, uint8 restartAfter);

/*************************************************************************************

    Function :      int32 flmVerifySpawn(uint32 deviceMask, uint8 restartAfter)

    Parameters :    deviceMask -
                        Specifies which devices will be verified. See flmOpen for the
                        details of this parameter.

                    restartAfter -
                        Set to 1 to restart all devices after completion of the verify
                        process. Set to 0 if restart is not required.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   This is the multiple connection version of flVerify. Refer to
                    flVerify for more details.
                    <p>
                    NOTE: The return value indicates only whether the verify
                    threads started successfully or not. When verify completes,
                    flmGetDeviceError should be called to get the final status.
                    flmGetDeviceProgress should be used to check for completion.

*************************************************************************************/
TESTFLASH_API(int32) flmVerifySpawn(uint32 deviceMask, uint8 restartAfter);

/*************************************************************************************

    Function :      int32 flGetFlashInfo(uint16* sectors, uint16* sizeMbits,
                                         uint32* manId, uint32* devId)

    Parameters :    sectors -
                        Location for receiving the number of sectors in the NVM
                        device.

                    sizeMbits -
                        Location for receiving the size of the NVM device in MBits.
                        <p>
                        Note that some chips have reserved sectors, which reduces
                        the amount of space available. This value is rounded to the
                        nearest integer to give the expected total size of the NVM.
                        For a more accurate value, use NvsCmd.

                    manId -
                        Location for receiving the manufacturer ID of the NVM device.
                        <p>
                        Note that for CSR102x ICs, the manufacturer ID cannot be
                        obtained, therefore this value is always set to 0.

                    devId -
                        Location for receiving the device ID of the NVM device.
                        <p>
                        Note that for CSR102x ICs, the device ID cannot be
                        obtained, therefore this value is always set to 0.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Gets the NVM information, including the number of sectors, size,
                    manufacturer ID, and device ID.
                    <p>
                    This function will fail if called when a threaded (spawned)
                    operation is in progress.

*************************************************************************************/
TESTFLASH_API(int32) flGetFlashInfo(uint16* sectors, uint16* sizeMbits,
                                    uint32* manId, uint32* devId);

/*************************************************************************************

    Function :      int32 flmGetDeviceFlashInfo(uint32 device, uint16* sectors,
                                                uint16* sizeMbits, uint32* manId,
                                                uint32* devId)

    Parameters :    device -
                        The index of the device for which to retrieve the NVM
                        information. E.g. to retrieve the information for the 3rd
                        device, the parameter is 2. For LPT-SPI transports only,
                        device indices start at 1, so the parameter value for the 3rd
                        device would be 3 in this case.

                    sectors -
                        See flGetFlashInfo.

                    sizeMbits -
                        See flGetFlashInfo.

                    manId -
                        See flGetFlashInfo.

                    devId -
                        See flGetFlashInfo.

    Returns :       See flGetFlashInfo.

    Description :   This is the multiple connection version of flGetFlashInfo. Refer
                    to flGetFlashInfo for more details.

*************************************************************************************/
TESTFLASH_API(int32) flmGetDeviceFlashInfo(uint32 device, uint16* sectors,
                                           uint16* sizeMbits, uint32* manId,
                                           uint32* devId);

/*************************************************************************************

    Function :      int32  flSetFlashType(uint8 type)

    Parameters :    type -
                        The type of NVM to use:
                        <table>
                            <tr><td>TFL_TYPE_STANDARD                <td> 0
                            <tr><td>TFL_TYPE_SPIF                    <td> 1
                            <tr><td>TFL_TYPE_SQIF                    <td> 2
                            <tr><td>TFL_TYPE_MTP                     <td> 3
                            <tr><td>TFL_TYPE_OTP                     <td> 4
                            <tr><td>TFL_TYPE_SMEM                    <td> 5
                            <tr><td>TFL_TYPE_E2                      <td> 6
                        </table>

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Sets the NVM type to the variant specified. Call this function
                    before any other functions that operate on NVM (unless the
                    default NVM type is appropriate).
                    <p>
                    The default NVM type for BlueCore ICs is TFL_TYPE_STANDARD. This
                    flash memory type is generally used for device firmware, and
                    depending on the chip type, this is either internal to the chip
                    package, or mandatory external flash. TFL_TYPE_SPIF means external
                    "Serial SPI Flash", and TFL_TYPE_SQIF means external "Serial Quad
                    Interface Flash". Some devices may use standard flash and either
                    SPIF or SQIF. Some devices can use generic NVM, which exists in
                    either flash(SMEM), One-Time-Programmable (OTP), or
                    Multi-Time-Programmable (MTP) forms.
                    <p>
                    For CSRA681xx, QCC302x/3x and QCC512x ICs, the default flash type
                    is TFL_TYPE_SQIF.
                    <p>
                    For CSR102x ICs, one of TFL_TYPE_MTP, TFL_TYPE_OTP and
                    TFL_TYPE_SMEM must be selected.

*************************************************************************************/
TESTFLASH_API(int32) flSetFlashType(uint8 type);

/*************************************************************************************

    Function :      int32 flmSetFlashType(uint32 deviceMask, uint8 type)

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    type -
                        See flSetFlashType.

    Returns :       See flSetFlashType.

    Description :   This is the multiple connection version of flSetFlashType. Refer
                    to flSetFlashType for more details.

*************************************************************************************/
TESTFLASH_API(int32) flmSetFlashType(uint32 deviceMask, uint8 type);

/*************************************************************************************

    Function :      int32  flSetPios(uint8 pioSclk, uint8 pioMiso, uint8 reserved)

    Parameters :    pioSclk -
                        The PIO that is connected to SPI SCLK.

                    pioMiso -
                        The PIO that is connected to SPI MISO.

                    reserved -
                        Reserved parameter (currently unused).

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Sets the PIOs that are connected to the serial flash.

*************************************************************************************/
TESTFLASH_API(int32) flSetPios(uint8 pioSclk, uint8 pioMiso, uint8 reserved);

/*************************************************************************************

    Function :      int32  flmSetPios(uint32 deviceMask, uint8 pioSclk, uint8 pioMiso,
                                      uint8 reserved)

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    pioSclk -
                        The PIO that is connected to SPI SCLK.

                    pioMiso -
                        The PIO that is connected to SPI MISO.

                    reserved -
                        Reserved parameter (currently unused).

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Sets the PIOs that are connected to the serial flash.

*************************************************************************************/
TESTFLASH_API(int32) flmSetPios(uint32 deviceMask, uint8 pioSclk, uint8 pioMiso,
                                uint8 reserved);

/*************************************************************************************

    Function :      int32  flSetSubsysBank(uint8 subSys, uint8 bank)

    Parameters :    subSys -
                        The subsystem targetted. Values are device dependent.
                        <p>
                        For CSRA681xx devices, typical values include:
                        <table>
                            <tr><td>3 = Audio
                            <tr><td>4 = Application
                        </table>
                        <p>
                        For QCC302x/3x and QCC512x devices, the only supported value
                        is 4 (Application).

                    bank -
                        The bank of the attached flash memory, e.g. 0.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Sets the subsystem and flash bank parameters.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx devices, the application subsystem flash must
                    be programmed with a valid image before operations can be
                    performed on the audio subsystem flash.

*************************************************************************************/
TESTFLASH_API(int32) flSetSubsysBank(uint8 subSys, uint8 bank);

/*************************************************************************************

    Function :      int32  flmSetSubsysBank(uint32 deviceMask, uint8 subSys,
                                            uint8 bank)

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    subSys -
                        The subsystem targetted. Values are device dependent.
                        <p>
                        For CSRA681xx devices, typical values include:
                        <table>
                            <tr><td>3 = Audio
                            <tr><td>4 = Application
                        </table>
                        <p>
                        For QCC302x/3x and QCC512x devices, the only supported value
                        is 4 (Application).

                    bank -
                        The bank of the attached flash memory, e.g. 0.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Sets the subsystem and flash bank parameters.
                    <p>
                    This function is not applicable for (and has no effect on) 
                    BlueCore or Bluetooth low energy ICs.
                    <p>
                    NOTE: For CSRA681xx devices, the application subsystem flash must
                    be programmed with a valid image before operations can be
                    performed on the audio subsystem flash.

*************************************************************************************/
TESTFLASH_API(int32) flmSetSubsysBank(uint32 deviceMask, uint8 subSys, uint8 bank);

/*************************************************************************************

    Function :      int32  flSetSqifAccess(const char* fileName)

    Parameters :    filename -
                        The name of the file containing the initialisation settings
                        allowing use of SQIF parts from alternative manufacturers.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Defines the single override file to be used to enable access to
                    the specified SQIF.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx devices, the application subsystem flash must
                    be programmed with a valid image before operations can be
                    performed on the audio subsystem flash.

*************************************************************************************/
TESTFLASH_API(int32) flSetSqifAccess(const char* fileName);

/*************************************************************************************

    Function :      int32  flmSetSqifAccess(uint32 deviceMask, const char* fileName);

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    fileName -
                        The name of the file containing the initialisation settings
                        allowing use of SQIF parts from alternative manufacturers.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Defines the single override file to be used to enable access to
                    the specified SQIF.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx devices, the application subsystem flash must
                    be programmed with a valid image before operations can be
                    performed on the audio subsystem flash.

*************************************************************************************/
TESTFLASH_API(int32) flmSetSqifAccess(uint32 deviceMask, const char* fileName);

/*************************************************************************************

    Function :      int32 flSetSecurityKey(const char* keyValOrFileName)

    Parameters :    keyValOrFileName -
                        Either the key value, or the path and filename of a text file
                        that contains the key value.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Writes the security key value to the device.
                    <p>
                    The key value is a sequence of exactly 32 characters selected
                    from the HEX digits (0123456789ABCDEF).
                    If the keyValOrFileName is not a 32 digit HEX value, then it is
                    assumed to be the name of a file, and this file will be opened
                    to read the key value as a 32 digit HEX string.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx, QCC302x/3x, and QCC512x devices, the key
                    is 128 bits long (32 HEX digits).
                    Use of the security key is enabled by calling flEnableSecurity.

*************************************************************************************/
TESTFLASH_API(int32) flSetSecurityKey(const char* keyValOrFileName);

/*************************************************************************************

    Function :      int32 flmSetSecurityKey(uint32 deviceMask, 
                                            const char* keyValOrFileName);

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    keyValOrFileName -
                        Either the key value, or the path and filename of a text file
                        that contains the key value.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Writes the security key value to the devices.
                    <p>
                    The key value is a sequence of exactly 32 characters selected
                    from the HEX digits (0123456789ABCDEF).
                    If the keyValOrFileName is not a 32 digit HEX value, then it is
                    assumed to be the name of a file, and this file will be opened
                    to read the key value as a 32 digit HEX string.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx, QCC302x/3x, and QCC512x devices, the key
                    is 128 bits long (32 HEX digits).
                    Use of the security key is enabled by calling flmEnableSecurity.

*************************************************************************************/
TESTFLASH_API(int32) flmSetSecurityKey(uint32 deviceMask, 
                                       const char* keyValOrFileName);

/*************************************************************************************

    Function :      int32 flEnableSecurity(uint32 options)

    Parameters :    options -
                        A value to define what security aspects are to be enabled,
                        where each bit specifies a security aspect as follows:
                        <table>
                            <tr><td>Bit 0     <td>Encryption
                            <tr><td>Bit 1     <td>Debug Transport Lock
                            <tr><td>Bit 2     <td>USB Debug disabled
                            <tr><td>Bit 3..31 <td>Reserved. Must be set to zero
                                                  unless advised otherwise by QTIL.
                        </table>

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Enables one or more security aspects.
                    <p>
                    Security aspects enabled with this function become active after 
                    the next power cycle, device reset, or flClose/flCloseRet call.
                    See flResetAndStart for details of performing a device reset.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx, QCC302x/3x, and QCC512x devices, the same
                    key (written with flSetSecurityKey) is used for debug transport
                    locking and encryption, and the Debug Transport Lock bit has no
                    effect unless the Encryption bit is also set (or encryption has
                    been previously enabled). Once both the encryption and debug
                    transport lock security aspects are active, the correct key will
                    need to be supplied when opening a connection. See flOpenUnlock
                    for details.
                    <p>
                    NOTE: If the "USB Debug disabled" option is set, USB Debug will
                    be permanently disabled, with no means to re-enable it. It is
                    not recommended that this is done unless another debug transport
                    (such as TRB) is available.

*************************************************************************************/
TESTFLASH_API(int32) flEnableSecurity(uint32 options);

/*************************************************************************************

    Function :      int32 flmEnableSecurity(uint32 deviceMask, uint32 options);

    Parameters :    deviceMask -
                        A bit mask of the devices to be set. See flmOpen for the
                        details of this parameter.

                    options -
                        see flEnableSecurity for details.

    Returns :       <table>
                        <tr><td>TFL_OK              <td>0
                        <tr><td>TFL_ERROR           <td>-1
                    </table>

    Description :   Enables one or more security aspects.
                    <p>
                    Security aspects enabled with this function become active after
                    the next power cycle, device reset, or flmClose/flmCloseRet call.
                    See flmResetAndStart for details of performing a device reset.
                    <p>
                    This function is not applicable for BlueCore or Bluetooth low
                    energy ICs.
                    <p>
                    NOTE: For CSRA681xx, QCC302x/3x, and QCC512x devices, the same
                    key (written with flmSetSecurityKey) is used for debug transport
                    locking and encryption, and the Debug Transport Lock bit has no
                    effect unless the Encryption bit is also set (or encryption has
                    been previously enabled). Once both the encryption and debug
                    transport lock security aspects are active, the correct key will
                    need to be supplied when opening a connection. See flmOpenUnlock
                    for details.
                    <p>
                    NOTE: If the "USB Debug disabled" option is set, USB Debug will
                    be permanently disabled, with no means to re-enable it. It is
                    not recommended that this is done unless another debug transport
                    (such as TRB) is available.

*************************************************************************************/
TESTFLASH_API(int32) flmEnableSecurity(uint32 deviceMask, uint32 options);

/*************************************************************************************

    Function :      uint16 flGetChipId(void)

    Parameters :    None

    Returns :       The chip ID, 0 if unknown.

    Description :   Returns the ID of the connected device (if known).

*************************************************************************************/
TESTFLASH_API(uint16) flGetChipId(void);

/*************************************************************************************

    Function :      uint16 flmGetChipId(uint32 device)

    Parameters :    device -
                        The index of the device for which to retrieve the chip ID.
                        E.g. to retrieve the chip ID for the 3rd device, the parameter
                        is 2. For LPT-SPI transports only, device indices start at 1,
                        so the parameter value for the 3rd device would be 3 in this
                        case.

    Returns :       See flGetChipId.

    Description :   This is the multiple connection version of flGetChipId.
                    See flGetChipId for details.

*************************************************************************************/
TESTFLASH_API(uint16) flmGetChipId(uint32 device);



/* DEPRECATED_START */
#define init_flash               flOpen
#define read_program_files       flReadProgramFiles
#define program_flash_block      flProgramBlock
#define erase_flash              flErase
#define gang_program_flash_block flGangProgramBlock
#define get_bit_error_field      flGetBitErrorField
#define program_flash_spawn      flProgramSpawn
#define gang_program_flash_spawn flGangProgramSpawn
#define get_spawned_progress     flGetProgress
#define get_spawned_error        flGetLastError
#define reset_and_start          flResetAndStart
#define close_flash              flClose
#define get_version              flGetVersion
#define flGetAvailableSpiPorts   flGetAvailablePorts
#define flmGetAvailableSpiPorts  flmGetAvailablePorts
#define flInit                   flOpen
#define flInitTrans              flOpenTrans
#define flInitSpiUnlock          flOpenUnlock
#define flInitSpiUnlockTrans     flOpenUnlockTrans
#define flmOpenSpiUnlock         flmOpenUnlock
#define flmOpenSpiUnlockTrans    flmOpenUnlockTrans
#define flmConvertSpiPort        flmConvertPort
/* DEPRECATED_END */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCTestFlashh */
