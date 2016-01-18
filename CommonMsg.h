///////////////////////////////////////////////////////////
//  CommonMsg.h
//  Placeholder for common SPI RF01 structures and definitions
//  Created on:      19-Jan-2015 12:00:00 PM
//  Original author: Jacques Dubé, Ing. Jr.
//  Co-author: Simon Defayette, Ing.
///////////////////////////////////////////////////////////

/*! \file CommonMsg.h
\brief Definitions of MACROS, constants, structures and datas union used by the SPI RF01 technology.

The following messages are used as command in the \ref MW_TagInvokeCommand function:
\par
 - \ref TAG_REQUEST
 - \ref TAG_HIBERNATE
 - \ref IDENTIFY_ACTIVE_TAG

The following messages are used as command in the \ref MW_TagInvokeCommandEx function:
\par
 - \ref TAG_CONFIG
 - \ref TAG_STATUS
 - \ref READER_INFOS
*/
#ifndef _COMMONMSG_H_060220151108
#define _COMMONMSG_H_060220151108

// ---------------------------------------------------------------------------
// READER COMMAND'S ID
// ---------------------------------------------------------------------------
/*! \brief Basic minimal msg, every 2 sec.

- Direction:  tag->reader
- Format: Header + MsgType + TagID + Type + Checksum (8 bytes)
- Size: \ref MSG_SIZE_TAG_BEACON
*/
#define TAG_BEACON 0x01

/*! \brief Extended msg, sent every 10 minutes.

This message contains temperature (on 10 bits), battery level, an error flag, two spare flags, and next maintenance date.

- Direction:  tag->reader
- Format: Header + MsgType + TagID + [6 spare bits + TemperatureHi 2bit] + TemperatureLo + [2 spare bits + Error flag 1bit + BatLevel 5bit] +
  MaintenanceHi + MaintenanceLo + Checksum
- Size: \ref MSG_SIZE_TAG_EXTENDED_INFO
*/
#define TAG_EXTENDED_INFO 0x02

/*! \brief On demand msg, by proximity reader.

Tag shall return it's ID, device kind & FW version (2 bytes), batch number (2 bytes).
- Direction:  tag->reader
- Format: Header + MsgType + TagID + [DevKind 2bits + RevMajor 6bits] + RevMinor + BatchHi + BatchLo + Checksum
- Size: \ref MSG_SIZE_TAG_FW_INFO
@see DEVICE_KIND_TAG, DEVICE_KIND_ANTENNA, DEVICE_KIND_PROGRAMMER
*/
#define TAG_FW_INFO 0x03

/*! \brief On demand msg, by proximity reader, during manufacturing process.

Any information with a value of -1 (0xFFFF) shall remains untouched in the event of writing information to the tag.
Tag shall confirm by resending same msg with received data.

- Programmed information: Tag ID, type, batch, number, maintenance date, beacon rate, power, and data rate.
- Direction:  bidirectional
- Format: Header + MsgType + TagID + Type + BatchHi + BatchLo + MaintenanceHi + MaintenanceLo + BeaconHi + BeaconLo + Power + Sensors + Checksum
- Sensors byte is used to show installed sensors and device data rate. The bit mapping is as follow:
| | | Sensors byte bits | | |
|:-------------:|:------:|:----------------:|:----------:|:-----------:|
| 7 6           | 5 4 3  | 2                | 1          | 0           |
| 2 bits        | 3 bits | 1 bit            | 1 bit      | 1 bit       |
| Data rate     | Unused | Temperature flag | Light flag | Motion flag |
| Read/(Write) |        | Read Only        | Read Only  | Read Only   |
- Size: \ref MSG_SIZE_TAG_CONFIG
@see DATA_RATE_250KBPS, DATA_RATE_2MBPS, DATA_RATE_1MBPS
@note In this version of the SDK, the data rate is READ ONLY. Further certifications are required to allow this option.
@warning Setting a lower data rate increase tag's detection range, but also reduce battery's life and increase messages collisions risks
*/
#define TAG_CONFIG 0x04

/*! \brief On demand msg, by proximity reader, to force a msg to be sent by tag.

Useful to test and validate. Tag shall reply with the requested msg.
- Direction:  reader->tag
- Format: Header + MsgType + TagID + MsgRequested + Checksum
- Size: \ref MSG_SIZE_TAG_REQUEST
*/
#define TAG_REQUEST 0x05

/*! \brief On demand msg, by proximity reader, to force a tag into or out of hibernation.

Useful for air transportation, and shelving. Tag shall not reply.  Application shall validate the action requested.
- Direction:  bidirectional
- Format: Header + MsgType + TagID + HibernationState + Checksum
- Size: \ref MSG_SIZE_TAG_HIBERNATE
*/
#define TAG_HIBERNATE 0x06

/*! \brief On demand msg and sent periodically

This informs the application about any errors encountered by retrieving all failure counters. All counters are 2 bytes. If any
of these values changes during the life cycle of the tag, an error flag will be raised so that the user can take appropriate
actions. If this message is used to write datas to the tag, the errors counters will be reseted only if a value of 0 is passed. Any
other values will be ignored and in all case, the error flag will be dropped.

- Direction:  bidirectional
- Format: Header + MsgType + TagID + FlashErrorCount + WakeUpErrorCount + LowBatErrorCount + WhatchdogResetCount + XtalFaultCount +
 NrfRetryCount + Checksum
- Size: \ref MSG_SIZE_TAG_STATUS
*/
#define TAG_STATUS 0x07

/*! \brief On demand msg and sent upon alert detection, at beacon rate.

MovementDuration shall be a value between 0 and 255.  0 means no movement, 255 maximum durations (of about 1 second).  This is used to allow the application to take action only if exceeding a defined threshold.
- Direction:  tag->reader
- Format: Header + MsgType + TagID + MovementDuration + [7 spares bits + LightDetected (1 bit)] + Checksum
- Size: \ref MSG_SIZE_TAG_ALERT
*/
#define TAG_ALERT 0x08

/*! \brief (Read only) Message used by a reader to append extra datas to a message sent to a device connected on the UART

An antenna will only wrap it's frame if it's address has been set (is not 0). This message exist only in the scope of the device listening to the reader.
It should not be used to send informations to the antennas.
- Direction: reader->PC
- Format: Header + MsgType + AddressHi + AddressLo + [Wrapped message (Without header and checksum)] + Checksum
- Size: \ref MSG_SIZE_TAG_WRAPPER + \c Wrapped \c message \c size
@see MSG_SIZE_TAG_ALERT, MSG_SIZE_TAG_BEACON, MSG_SIZE_TAG_CONFIG, MSG_SIZE_TAG_EXTENDED_INFO, MSG_SIZE_TAG_FW_INFO, MSG_SIZE_TAG_HIBERNATE, MSG_SIZE_TAG_REQUEST, MSG_SIZE_TAG_STATUS, MSG_SIZE_READER_INFOS
*/
#define TAG_WRAPPER 0x09

/*! \brief Antenna and Programmer only. Message used to recover or set the reader message address.

The address of the reader can be anything except \ref NO_PARAM. An antenna address is considered not set if its value is 0. The antenna's address has no impact on the communication channel. It is only a token
to help user to identify the source of the message upon the reception of a frame. The address is also not unique, which is useful to setup a detection zone by grouping antennas together.
- Direction: PC <-> reader
- Format: Header + MsgType + TagID + AddressHi + AddressLo + Checksum
- Size: \ref MSG_SIZE_READER_INFOS
@note Currently, the tag ID of the message is ignored and kept here for future uses.
*/
#define READER_INFOS 0x0A

// ---------------------------------------------------------------------------
// READER'S SOFTWARE COMMAND. HIGHER VALUE TO PREVENT MIXING WITH COMMON HARDWARE COMMAND.
// ---------------------------------------------------------------------------
/*! \brief Recover the firmware informations of the tag currently being modified by the programmer

This message is software specific. The active tag is set when a tag trigger the
programmer's hall sensor. The message returned is the same message the tag used
to identify itself to the hardware.

- Direction: None
- Format & size: see \ref TAG_FW_INFO
*/
#define IDENTIFY_ACTIVE_TAG 0x100

// ---------------------------------------------------------------------------
// CONSTANTS
// ---------------------------------------------------------------------------
#define POWER_18 3 ///< Tag's power to -18 dBm (0b11)
#define POWER_12 2 ///< Tag's power to -12 dBm (0b10)
#define POWER_6  1 ///< Tag's power to  -6 dBm (0b01)
#define POWER_0  0 ///< Tag's power to   0 dBm (0b00)
#define DEFAULT_BEACON 2000 ///< Default value for the beaconrate
#define DEFAULT_POWER POWER_0 ///< Default value for the power
#define DATA_RATE_250KBPS 2 ///< Tag's data rate speed to 250Kbps (0b11)
#define DATA_RATE_2MBPS   1 ///< Tag's data rate speed to 2Mbps   (0b01)
#define DATA_RATE_1MBPS   0 ///< Tag's data rate speed to 1Mbps   (0b00)
#define DEFAULT_DATA_RATE DATA_RATE_1MBPS ///< Default value for the data rate speed
#define NO_DATE 0 ///< Values of a NULL date
#define DEFAULT_MAINTENANCE_DATE NO_DATE ///< Default value for the maintenance date. @see TAG_DATE for informations about the bit map.
#define RFID_MSG_HEADER 0x0D ///< Header for every SPI RF01 frame. Used for synchronisation
#define NO_PARAM -1 ///< Default values for undesired parameters that can't be NULL
#define MSG_TYPE 1 ///< Position of the message's type in all protocols
#define REQUEST_TYPE 6 ///< Position of the requested message's type in a \ref TAG_REQUEST message
#define DEVICE_KIND_TAG 0 ///< Device identifier : Tag
#define DEVICE_KIND_ANTENNA 1 ///< Device identifier : Antenna
#define DEVICE_KIND_PROGRAMMER 2 ///< Device identifier : Programmer
#define MSG_SIZE_TAG_ALERT 9 ///< Size of the \ref TAG_ALERT message, in bytes
#define MSG_SIZE_TAG_BEACON 8 ///< Size of the \ref TAG_BEACON message, in bytes
#define MSG_SIZE_TAG_CONFIG 16 ///< Size of the \ref TAG_CONFIG message, in bytes
#define MSG_SIZE_TAG_EXTENDED_INFO 12 ///< Size of the \ref TAG_EXTENDED_INFO message, in bytes
#define MSG_SIZE_TAG_FW_INFO 11 ///< Size of the \ref TAG_FW_INFO message, in bytes
#define MSG_SIZE_TAG_HIBERNATE 8 ///< Size of the \ref TAG_HIBERNATE message, in bytes
#define MSG_SIZE_TAG_REQUEST 8 ///< Size of the \ref TAG_REQUEST message, in bytes
#define MSG_SIZE_TAG_STATUS 19 ///< Size of the \ref TAG_STATUS message, in bytes
#define MSG_SIZE_TAG_WRAPPER 5 ///< Number of bytes to add to the size of the the message that is wrapped inside a \ref TAG_WRAPPER message
#define MSG_SIZE_READER_INFOS 9 ///< Size of the \ref READER_INFOS message, in bytes

// ---------------------------------------------------------------------------
// RETURN STATUS
// ---------------------------------------------------------------------------
#define MW_OK 0 ///< Function call return status : OK
#define MW_ERROR -1 ///< Function call return status : Generic error
#define MW_ERROR_INVALID_PARAMETER -2 ///< Function call return status : A command's parameter is invalid
#define MW_ERROR_INVALID_COMMAND -3 ///< Function call return status : A command to be sent is invalid
#define MW_ERROR_BUFFER_TOO_SMALL -4 ///< Function call return status : The lenght of the buffer is to small for the needs of the caller's function
#define MW_ERROR_TIMEOUT -5 ///< Function call return status : A command had to return before getting a correct answer from the controller

// ---------------------------------------------------------------------------
// STRUCTURES AND UNIONS DEFINITIONS
// ---------------------------------------------------------------------------
/** \brief Date protocol used by the SPI RF01 tags, yy/mm/dd
 *
 * This union map the binary representation of the SPI RF01 date's format
 * into a more user friendly representation. The date is encoded in 16 bits in the
 * following format:
 *
 * | | Bits | |
 * |:---------------------------:|:--------------:|:----------------------:|
 * | 15 14 13 12 11              | 10 9 8 7       | 6 5 4 3 2 1 0          |
 * | 5 bits                      | 4 bits         | 7 bits                 |
 * | Day                         | Month          | Year                   |
 *
 * \remarks This bit field does not represents how the date is stored in the \ref TagStruct.
 */
union TAG_DATE
{
#ifdef WIN32
#ifndef uint16_t
typedef unsigned __int16 uint16_t;
#endif
#endif
	//! \brief Structures mapping the bits in date's protocol with desired values
	struct DATE_MAP
	{
		//! @cond Doxygen_Suppress
		uint16_t year  :7;
		uint16_t month :4;
		uint16_t day   :5;
		//! @endcond
	} stDateMap; ///< Structures mapping the bits in the above format with desired values
	uint16_t uiDate; ///< Raw representation of the 16 bits variable
};
#endif // _COMMONMSG_H_060220151108

