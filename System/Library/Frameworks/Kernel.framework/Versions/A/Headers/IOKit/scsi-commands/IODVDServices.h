/*
 * Copyright (c) 1998-2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */


#ifndef _IOKIT_IO_DVD_SERVICES_H_
#define _IOKIT_IO_DVD_SERVICES_H_

#if defined(KERNEL) && defined(__cplusplus)


//ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	Includes
//ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

// IOKit includes
#include <IOKit/IOTypes.h>

// Generic IOKit storage related headers
#include <IOKit/storage/IODVDTypes.h>
#include <IOKit/storage/IODVDBlockStorageDevice.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi-commands/IOSCSIPeripheralDeviceType05.h>

class IOMemoryDescriptor;

// Use this switch to turn off the data cache.
#define _DVD_USE_DATA_CACHING_		1


//ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	Class Declaration
//ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

class IODVDServices : public IODVDBlockStorageDevice
{
	
	OSDeclareDefaultStructors ( IODVDServices )
	
#if (_DVD_USE_DATA_CACHING_)
	// Data Cache members
	bool				fUseDataCache;			// Indicates if cache should be used.  This
												// will be set to false if all the necessary support
												// for the data cache could not be allocated or
												// initialized.
	UInt8 *				fDataCacheStorage;		// Storage space for the cache
	UInt32				fDataCacheStartBlock;	// Starting block of the data in the cache.
	UInt32				fDataCacheBlockCount;	// Number of contiguous blocks in the cache  
												// starting with fDataCacheStartBlock. A value of
												// zero in this member indicates that all data
												// in the cache is invalid.
	
	IOSimpleLock *		fDataCacheLock; 		// This is the lock for preventing multiple access 
												// while manipulating the data cache.
#endif
	
protected:
	
	OSSet *								fClients;
	IOSCSIPeripheralDeviceType05 *		fProvider;
	
	virtual void	free ( void );			
	
    // Reserve space for future expansion.
    struct IODVDServicesExpansionData { };
    IODVDServicesExpansionData * fIODVDServicesReserved;
	
public:
	
	static void			AsyncReadWriteComplete ( void * 		clientData,
		                          				 IOReturn		status,
		                      					 UInt64 		actualByteCount );
	
	virtual IOReturn	doAsyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt32					block,
											UInt32					nblks,
											IOStorageCompletion		completion );

	virtual IOReturn	doSyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt32					block,
											UInt32					nblks );

    virtual IOReturn	doEjectMedia ( void );

    virtual IOReturn	doFormatMedia ( UInt64 byteCapacity );

    virtual UInt32		doGetFormatCapacities ( UInt64 *	capacities,
    											UInt32		capacitiesMaxCount ) const;

    virtual IOReturn	doLockUnlockMedia ( bool doLock );

    virtual IOReturn	doSynchronizeCache ( void );
        
    virtual char *		getVendorString ( void );
    
    virtual char *		getProductString ( void );
    
    virtual char *		getRevisionString ( void );
    
    virtual char *		getAdditionalDeviceInfoString ( void );
    
    virtual IOReturn	reportBlockSize ( UInt64 * blockSize );
    
    virtual IOReturn	reportEjectability ( bool * isEjectable );
    
    virtual IOReturn	reportLockability ( bool * isLockable );
    
    virtual IOReturn	reportMediaState ( bool * mediaPresent, bool * changed );
    
    virtual IOReturn	reportPollRequirements ( 	bool * pollIsRequired,
    												bool * pollIsExpensive );
    
    virtual IOReturn	reportMaxReadTransfer ( UInt64 blockSize, UInt64 * max );
    
    virtual IOReturn	reportMaxValidBlock ( UInt64 * maxBlock );
    
    virtual IOReturn	reportMaxWriteTransfer ( UInt64 blockSize, UInt64 * max );
    
    virtual IOReturn	reportRemovability ( bool * isRemovable );
    
    virtual IOReturn	reportWriteProtection ( bool * isWriteProtected );

	/* CD Specific */
	virtual IOReturn	doAsyncReadCD ( IOMemoryDescriptor * buffer,
										UInt32 block,
										UInt32 nblks,
										CDSectorArea sectorArea,
										CDSectorType sectorType,
										IOStorageCompletion completion );
	
	virtual IOReturn	readISRC ( UInt8 track, CDISRC isrc );
	
	virtual IOReturn	readMCN ( CDMCN mcn);
	
	virtual IOReturn	readTOC ( IOMemoryDescriptor * buffer );
	
	virtual IOReturn	audioPause ( bool pause );
	
	virtual IOReturn	audioPlay ( CDMSF timeStart, CDMSF timeStop );
	
	virtual IOReturn	audioScan ( CDMSF timeStart, bool reverse );
	
	virtual IOReturn	audioStop ( void );
	
	virtual IOReturn	getAudioStatus ( CDAudioStatus * status );
	
	virtual IOReturn	getAudioVolume ( UInt8 * leftVolume, UInt8 * rightVolume );
	
	virtual IOReturn	setAudioVolume ( UInt8 leftVolume, UInt8 rightVolume );

	virtual IOReturn	getSpeed ( UInt16 * kilobytesPerSecond );
	
	virtual IOReturn	setSpeed ( UInt16 kilobytesPerSecond );
	
	/* DVD Specific */
	virtual UInt32			getMediaType		( void );
    
	virtual IOReturn		reportKey			( IOMemoryDescriptor * buffer,
												  const DVDKeyClass keyClass,
												  const UInt32 lba,
												  const UInt8 agid,
												  const DVDKeyFormat keyFormat );
												  
	virtual IOReturn		sendKey				( IOMemoryDescriptor * buffer,
												  const DVDKeyClass keyClass,
												  const UInt8 agid,
												  const DVDKeyFormat keyFormat );

	virtual IOReturn		readDVDStructure 	( 	IOMemoryDescriptor * 		buffer,
													const UInt8					structureFormat,
													const UInt32				logicalBlockAddress,
													const UInt8					layer,
													const UInt8 				agid );

	virtual IOReturn 	message ( UInt32 type, IOService * provider, void * argument );
    virtual IOReturn	setProperties ( OSObject * properties );

	/* User Client Specific */
	virtual bool start ( IOService * provider );
	virtual bool open ( IOService * client, IOOptionBits options, IOStorageAccess access );
	
	virtual bool handleOpen	( IOService * client, IOOptionBits options, void * access );
	virtual void handleClose ( IOService * client, IOOptionBits options );
	virtual bool handleIsOpen ( const IOService * client ) const;

	/* Added with 10.1.3 */
	virtual IOReturn	readTOC (	IOMemoryDescriptor *	buffer,
									CDTOCFormat				format,
									UInt8					msf,
									UInt8					trackSessionNumber,
									UInt16 *				actualByteCount ); 
	
	/* Added with 10.1.3 */
	virtual IOReturn	readDiscInfo (	IOMemoryDescriptor *	buffer,
										UInt16 *				actualByteCount );
	
	/* Added with 10.1.3 */
	virtual IOReturn	readTrackInfo (	IOMemoryDescriptor *	buffer,
										UInt32					address,
										CDTrackInfoAddressType	addressType,
										UInt16 *				actualByteCount );
	
private:
	
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IODVDServices, 1 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 2 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 3 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 4 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 5 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 6 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 7 );
    OSMetaClassDeclareReservedUnused ( IODVDServices, 8 );
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif /* _IOKIT_IO_DVD_SERVICES_H_ */