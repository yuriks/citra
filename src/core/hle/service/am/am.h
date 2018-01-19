// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <functional>
#include <string>
#include "common/common_types.h"
#include "core/file_sys/cia_container.h"
#include "core/file_sys/file_backend.h"
#include "core/hle/result.h"

namespace Service {
namespace FS {
enum class MediaType : u32;
}
}

namespace Service {

class Interface;

namespace AM {

namespace ErrCodes {
enum {
    CIACurrentlyInstalling = 4,
    InvalidTID = 31,
    EmptyCIA = 32,
    InvalidTIDInList = 60,
    InvalidCIAHeader = 104,
};
} // namespace ErrCodes

enum class CIAInstallState : u32 {
    InstallStarted,
    HeaderLoaded,
    CertLoaded,
    TicketLoaded,
    TMDLoaded,
    ContentWritten,
};

enum class InstallStatus : u32 {
    Success,
    ErrorFailedToOpenFile,
    ErrorFileNotFound,
    ErrorAborted,
    ErrorInvalid,
    ErrorEncrypted,
};

// Progress callback for InstallCIA, recieves bytes written and total bytes
using ProgressCallback = void(size_t, size_t);

// A file handled returned for CIAs to be written into and subsequently installed.
class CIAFile final : public FileSys::FileBackend {
public:
    explicit CIAFile(Service::FS::MediaType media_type) : media_type(media_type) {}
    ~CIAFile() {
        Close();
    }

    ResultVal<size_t> Read(u64 offset, size_t length, u8* buffer) const override;
    ResultVal<size_t> WriteTitleMetadata(u64 offset, size_t length, const u8* buffer);
    ResultVal<size_t> WriteContentData(u64 offset, size_t length, const u8* buffer);
    ResultVal<size_t> Write(u64 offset, size_t length, bool flush, const u8* buffer) override;
    u64 GetSize() const override;
    bool SetSize(u64 size) const override;
    bool Close() override;
    void Flush() const override;

private:
    // Whether it's installing an update, and what step of installation it is at
    bool is_update = false;
    CIAInstallState install_state = CIAInstallState::InstallStarted;

    // How much has been written total, CIAContainer for the installing CIA, buffer of all data
    // prior to content data, how much of each content index has been written, and where the CIA
    // is being installed to
    u64 written = 0;
    FileSys::CIAContainer container;
    std::vector<u8> data;
    std::vector<u64> content_written;
    Service::FS::MediaType media_type;
};

/**
 * Installs a CIA file from a specified file path.
 * @param path file path of the CIA file to install
 * @param update_callback callback function called during filesystem write
 * @returns bool whether the install was successful
 */
InstallStatus InstallCIA(const std::string& path,
                         std::function<ProgressCallback>&& update_callback = nullptr);

/**
 * Get the mediatype for an installed title
 * @param titleId the installed title ID
 * @returns MediaType which the installed title will reside on
 */
Service::FS::MediaType GetTitleMediaType(u64 titleId);

/**
 * Get the .tmd path for a title
 * @param media_type the media the title exists on
 * @param tid the title ID to get
 * @param update set true if the incoming TMD should be used instead of the current TMD
 * @returns string path to the .tmd file if it exists, otherwise a path to create one is given.
 */
std::string GetTitleMetadataPath(Service::FS::MediaType media_type, u64 tid, bool update = false);

/**
 * Get the .app path for a title's installed content index.
 * @param media_type the media the title exists on
 * @param tid the title ID to get
 * @param index the content index to get
 * @param update set true if the incoming TMD should be used instead of the current TMD
 * @returns string path to the .app file
 */
std::string GetTitleContentPath(Service::FS::MediaType media_type, u64 tid, u16 index = 0,
                                bool update = false);

/**
 * Get the folder for a title's installed content.
 * @param media_type the media the title exists on
 * @param tid the title ID to get
 * @returns string path to the title folder
 */
std::string GetTitlePath(Service::FS::MediaType media_type, u64 tid);

/**
 * Get the title/ folder for a storage medium.
 * @param media_type the storage medium to get the path for
 * @returns string path to the folder
 */
std::string GetMediaTitlePath(Service::FS::MediaType media_type);

/**
 * Scans the for titles in a storage medium for listing.
 * @param media_type the storage medium to scan
 */
void ScanForTitles(Service::FS::MediaType media_type);

/**
 * Scans all storage mediums for titles for listing.
 */
void ScanForAllTitles();

/**
 * AM::GetNumPrograms service function
 * Gets the number of installed titles in the requested media type
 *  Inputs:
 *      0 : Command header (0x00010040)
 *      1 : Media type to load the titles from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : The number of titles in the requested media type
 */
void GetNumPrograms(Service::Interface* self);

/**
 * AM::FindDLCContentInfos service function
 * Explicitly checks that TID high value is 0004008C or an error is returned.
 *  Inputs:
 *      1 : MediaType
 *    2-3 : u64, Title ID
 *      4 : Content count
 *      6 : Content IDs pointer
 *      8 : Content Infos pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void FindDLCContentInfos(Service::Interface* self);

/**
 * AM::ListDLCContentInfos service function
 * Explicitly checks that TID high value is 0004008C or an error is returned.
 *  Inputs:
 *      1 : Content count
 *      2 : MediaType
 *    3-4 : u64, Title ID
 *      5 : Start Index
 *      7 : Content Infos pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Number of content infos returned
 */
void ListDLCContentInfos(Service::Interface* self);

/**
 * AM::DeleteContents service function
 *  Inputs:
 *      1 : MediaType
 *    2-3 : u64, Title ID
 *      4 : Content count
 *      6 : Content IDs pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void DeleteContents(Service::Interface* self);

/**
 * AM::GetProgramList service function
 * Loads information about the desired number of titles from the desired media type into an array
 *  Inputs:
 *      1 : Title count
 *      2 : Media type to load the titles from
 *      4 : Title IDs output pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : The number of titles loaded from the requested media type
 */
void GetProgramList(Service::Interface* self);

/**
 * AM::GetProgramInfos service function
 *  Inputs:
 *      1 : u8 Mediatype
 *      2 : Total titles
 *      4 : TitleIDList pointer
 *      6 : TitleList pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void GetProgramInfos(Service::Interface* self);

/**
 * AM::GetDLCTitleInfos service function
 * Wrapper for AM::GetProgramInfos, explicitly checks that TID high value is 0004008C.
 *  Inputs:
 *      1 : u8 Mediatype
 *      2 : Total titles
 *      4 : TitleIDList pointer
 *      6 : TitleList pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void GetDLCTitleInfos(Service::Interface* self);

/**
 * AM::GetPatchTitleInfos service function
 * Wrapper for AM::GetProgramInfos, explicitly checks that TID high value is 0004000E.
 *  Inputs:
 *      1 : u8 Mediatype
 *      2 : Total titles
 *      4 : TitleIDList input pointer
 *      6 : TitleList output pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : TitleIDList input pointer
 *      4 : TitleList output pointer
 */
void GetPatchTitleInfos(Service::Interface* self);

/**
 * AM::ListDataTitleTicketInfos service function
 *  Inputs:
 *      1 : Ticket count
 *    2-3 : u64, Title ID
 *      4 : Start Index?
 *      5 : (TicketCount * 24) << 8 | 0x4
 *      6 : Ticket Infos pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Number of ticket infos returned
 */
void ListDataTitleTicketInfos(Service::Interface* self);

/**
 * AM::GetDLCContentInfoCount service function
 * Explicitly checks that TID high value is 0004008C or an error is returned.
 *  Inputs:
 *      0 : Command header (0x100100C0)
 *      1 : MediaType
 *    2-3 : u64, Title ID
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Number of content infos plus one
 */
void GetDLCContentInfoCount(Service::Interface* self);

/**
 * AM::DeleteTicket service function
 *  Inputs:
 *    1-2 : u64, Title ID
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void DeleteTicket(Service::Interface* self);

/**
 * AM::GetNumTickets service function
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Number of tickets
 */
void GetNumTickets(Service::Interface* self);

/**
 * AM::GetTicketList service function
 *  Inputs:
 *      1 : Number of TicketList
 *      2 : Number to skip
 *      4 : TicketList pointer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Total TicketList
 */
void GetTicketList(Service::Interface* self);

/**
 * AM::QueryAvailableTitleDatabase service function
 *  Inputs:
 *      1 : Media Type
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Boolean, database availability
 */
void QueryAvailableTitleDatabase(Service::Interface* self);

/**
 * AM::CheckContentRights service function
 *  Inputs:
 *      1-2 : Title ID
 *      3 : Content Index
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Boolean, whether we have rights to this content
 */
void CheckContentRights(Service::Interface* self);

/**
 * AM::CheckContentRightsIgnorePlatform service function
 *  Inputs:
 *      1-2 : Title ID
 *      3 : Content Index
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Boolean, whether we have rights to this content
 */
void CheckContentRightsIgnorePlatform(Service::Interface* self);

/**
 * AM::BeginImportProgram service function
 * Begin importing from a CTR Installable Archive
 *  Inputs:
 *      0 : Command header (0x04020040)
 *      1 : Media type to install title to
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2-3 : CIAFile handle for application to write to
 */
void BeginImportProgram(Service::Interface* self);

/**
 * AM::EndImportProgram service function
 * Finish importing from a CTR Installable Archive
 *  Inputs:
 *      0 : Command header (0x04050002)
 *      1-2 : CIAFile handle application wrote to
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void EndImportProgram(Service::Interface* self);

/**
 * AM::GetProgramInfoFromCia service function
 * Get TitleInfo from a CIA file handle
 *  Inputs:
 *      0 : Command header (0x04080042)
 *      1 : Media type of the title
 *      2-3 : File handle CIA data can be read from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2-8: TitleInfo structure
 */
void GetProgramInfoFromCia(Service::Interface* self);

/**
 * AM::GetSystemMenuDataFromCia service function
 * Loads a CIA file's SMDH data into a specified buffer
 *  Inputs:
 *      0 : Command header (0x04090004)
 *      1-2 : File handle CIA data can be read from
 *      3-4 : Output buffer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void GetSystemMenuDataFromCia(Service::Interface* self);

/**
 * AM::GetDependencyListFromCia service function
 * Loads a CIA's dependency list into a specified buffer
 *  Inputs:
 *      0 : Command header (0x040A0002)
 *      1-2 : File handle CIA data can be read from
 *      64-65 : Output buffer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void GetDependencyListFromCia(Service::Interface* self);

/**
 * AM::GetTransferSizeFromCia service function
 * Returns the total expected transfer size up to the CIA meta offset from a CIA
 *  Inputs:
 *      0 : Command header (0x040B0002)
 *      1-2 : File handle CIA data can be read from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2-3 : Transfer size
 */
void GetTransferSizeFromCia(Service::Interface* self);

/**
 * AM::GetCoreVersionFromCia service function
 * Returns the core version from a CIA
 *  Inputs:
 *      0 : Command header (0x040C0002)
 *      1-2 : File handle CIA data can be read from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Core version
 */
void GetCoreVersionFromCia(Service::Interface* self);

/**
 * AM::GetRequiredSizeFromCia service function
 * Returns the required amount of free space required to install a given CIA file
 *  Inputs:
 *      0 : Command header (0x040D0042)
 *      1 : Media type to install title to
 *      2-3 : File handle CIA data can be read from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2-3 : Required free space for CIA
 */
void GetRequiredSizeFromCia(Service::Interface* self);

/**
 * AM::GetMetaSizeFromCia service function
 * Returns the size of a given CIA's meta section
 *  Inputs:
 *      0 : Command header (0x04130002)
 *      1-2 : File handle CIA data can be read from
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 *      2 : Meta section size
 */
void GetMetaSizeFromCia(Service::Interface* self);

/**
 * AM::GetMetaDataFromCia service function
 * Loads meta section data from a CIA file into a given buffer
 *  Inputs:
 *      0 : Command header (0x04140044)
 *      1-2 : File handle CIA data can be read from
 *      3-4 : Output buffer
 *  Outputs:
 *      1 : Result, 0 on success, otherwise error code
 */
void GetMetaDataFromCia(Service::Interface* self);

/// Initialize AM service
void Init();

/// Shutdown AM service
void Shutdown();

} // namespace AM
} // namespace Service
