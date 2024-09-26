#pragma once
#include "ConfigFile.h"

/// \addtogroup ConfigurationFilesDescription 
/// <div style="page-break-after: always;"></div>
/// 
/// \section TomoConfig TomoConfig.cnf configuration file
/// 
/// ## Overview
/// 
/// This configuration files describes the parameters of all the Tomo sequences.
/// 
/// + **Name of the configuration file:** TomoConfig.cnf
/// + **Current revision code:** 1;
/// 
/// ## Parameter Description
/// 
/// This configuration file contains the following parameter rows: 
/// + TOMO_1F Narrow Sequence description;
/// + TOMO_1F Intermediate Sequence description;
/// + TOMO_1F Wide Sequence description;
/// + TOMO_2F Narrow Sequence description;
/// + TOMO_2F Intermediate Sequence description;
/// + TOMO_2F Wide Sequence description;
/// + TOMO_3F Narrow Sequence description;
/// + TOMO_3F Intermediate Sequence description;
/// + TOMO_3F Wide Sequence description;
/// + TOMO_4F Narrow Sequence description;
/// + TOMO_4F Intermediate Sequence description;
/// + TOMO_4F Wide Sequence description;
/// + TOMO_5F Narrow Sequence description;
/// + TOMO_5F Intermediate Sequence description;
/// + TOMO_5F Wide Sequence description;



ref class TomoConfig
{
public:

    enum class tomo_id {
        TOMO1F_NARROW = 0,
        TOMO1F_INTERMEDIATE,
        TOMO1F_WIDE,
        TOMO2F_NARROW,
        TOMO2F_INTERMEDIATE,
        TOMO2F_WIDE,
        TOMO3F_NARROW,
        TOMO3F_INTERMEDIATE,
        TOMO3F_WIDE,
        TOMO4F_NARROW,
        TOMO4F_INTERMEDIATE,
        TOMO4F_WIDE,
        TOMO5F_NARROW,
        TOMO5F_INTERMEDIATE,
        TOMO5F_WIDE,
        TOMO_ID_NUM
    };

    literal System::String^ FILENAME = "TomoConfig.cnf";
    literal int     FILE_REVISION = 1;

    literal int     PARAM_TOMO_HOME = 0;
    literal int     PARAM_TOMO_END = 1;
    literal int     PARAM_TOMO_FPS = 2;
    literal int     PARAM_TOMO_SAMPLES = 3;
    literal int     PARAM_TOMO_SKIP = 4;
    literal int     PARAM_TOMO_SPEED = 5;
    literal int     PARAM_TOMO_ACC = 6;
    literal int     PARAM_TOMO_DEC = 7;


    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_1F Narrow Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_1F Narrow sequence characteristics.
    /// 
    /// < TOMO1F_NARROW, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|825|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-825|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|11|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|150|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|150|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|150|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO1F_NARROW = "TOMO1F_NARROW";
    literal System::String^ PARAM_TOMO1F_NARROW_COMMENT = "Defines the Tomo 1F NARROW configuration";
    literal System::String^ PARAM_TOMO1F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO1F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO1F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO1F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_DEC_DEFAULT = "150";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_1F Intermediate Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_1F Intermediate sequence characteristics.
    /// 
    /// < TOMO1F_INTERMEDIATE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1300|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1300|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|13|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|200|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|200|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|200|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE = "TOMO1F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_COMMENT = "Defines the Tomo 1F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT = "200";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_1F Wide Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_1F Wide sequence characteristics.
    /// 
    /// < TOMO1F_WIDE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1900|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1900|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|19|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|200|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|200|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|200|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO1F_WIDE = "TOMO1F_WIDE";
    literal System::String^ PARAM_TOMO1F_WIDE_COMMENT = "Defines the Tomo 1F WIDE configuration";
    literal System::String^ PARAM_TOMO1F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO1F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO1F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO1F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_DEC_DEFAULT = "200";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_2F Narrow Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_2F Narrow sequence characteristics.
    /// 
    /// < TOMO2F_NARROW, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|825|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-825|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|11|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|300|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|600|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|600|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO2F_NARROW = "TOMO2F_NARROW";
    literal System::String^ PARAM_TOMO2F_NARROW_COMMENT = "Defines the Tomo 2F NARROW configuration";
    literal System::String^ PARAM_TOMO2F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO2F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO2F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO2F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO2F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO2F_NARROW_DEC_DEFAULT = "600";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_2F Intermediate Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_2F Intermediate sequence characteristics.
    /// 
    /// < TOMO2F_INTERMEDIATE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1300|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1300|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|13|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|400|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|800|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|800|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE = "TOMO2F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_COMMENT = "Defines the Tomo 2F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT = "800";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_2F Wide Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_2F Wide sequence characteristics.
    /// 
    /// < TOMO2F_WIDE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1900|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1900|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|19|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|400|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|800|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|800|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO2F_WIDE = "TOMO2F_WIDE";
    literal System::String^ PARAM_TOMO2F_WIDE_COMMENT = "Defines the Tomo 2F WIDE configuration";
    literal System::String^ PARAM_TOMO2F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO2F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO2F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO2F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_WIDE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_WIDE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_WIDE_DEC_DEFAULT = "800";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_3F Narrow Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_3F Narrow sequence characteristics.
    /// 
    /// < TOMO3F_NARROW, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|825|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-825|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|11|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|300|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|600|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|600|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO3F_NARROW = "TOMO3F_NARROW";
    literal System::String^ PARAM_TOMO3F_NARROW_COMMENT = "Defines the Tomo 3F NARROW configuration";
    literal System::String^ PARAM_TOMO3F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO3F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO3F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO3F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO3F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO3F_NARROW_DEC_DEFAULT = "600";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_3F Intermediate Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_3F Intermediate sequence characteristics.
    /// 
    /// < TOMO3F_INTERMEDIATE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1300|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1300|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|13|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|400|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|800|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|800|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE = "TOMO3F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_COMMENT = "Defines the Tomo 3F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT = "800";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_3F Wide Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_3F Wide sequence characteristics.
    /// 
    /// < TOMO3F_WIDE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|2608|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-2608|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|2|1:6|Sequence Fps|
    /// |Samples|Byte|24|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|435|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|869|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|869|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO3F_WIDE = "TOMO3F_WIDE";
    literal System::String^ PARAM_TOMO3F_WIDE_COMMENT = "Defines the Tomo 3F WIDE configuration";
    literal System::String^ PARAM_TOMO3F_WIDE_HOME_DEFAULT = "2608";
    literal System::String^ PARAM_TOMO3F_WIDE_END_DEFAULT = "-2608";
    literal System::String^ PARAM_TOMO3F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO3F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_WIDE_SPEED_DEFAULT = "435";
    literal System::String^ PARAM_TOMO3F_WIDE_ACC_DEFAULT = "869";
    literal System::String^ PARAM_TOMO3F_WIDE_DEC_DEFAULT = "869";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_4F Narrow Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_4F Narrow sequence characteristics.
    /// 
    /// < TOMO4F_NARROW, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|975|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-975|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|4|1:6|Sequence Fps|
    /// |Samples|Byte|11|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|3|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|600|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|800|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|800|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO4F_NARROW = "TOMO4F_NARROW";
    literal System::String^ PARAM_TOMO4F_NARROW_COMMENT = "Defines the Tomo 4F NARROW configuration";
    literal System::String^ PARAM_TOMO4F_NARROW_HOME_DEFAULT = "975";
    literal System::String^ PARAM_TOMO4F_NARROW_END_DEFAULT = "-975";
    literal System::String^ PARAM_TOMO4F_NARROW_FPS_DEFAULT = "4";
    literal System::String^ PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO4F_NARROW_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_NARROW_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_NARROW_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO4F_NARROW_DEC_DEFAULT = "800";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_4F Intermediate Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_4F Intermediate sequence characteristics.
    /// 
    /// < TOMO4F_INTERMEDIATE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1500|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1500|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|3|1:6|Sequence Fps|
    /// |Samples|Byte|13|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|3|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|600|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|600|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|600|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE = "TOMO4F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_COMMENT = "Defines the Tomo 4F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT = "1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT = "-1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT = "600";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_4F Wide Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_4F Wide sequence characteristics.
    /// 
    /// < TOMO4F_WIDE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|2550|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-2550|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|3|1:6|Sequence Fps|
    /// |Samples|Byte|24|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|675|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|900|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|900|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    literal System::String^ PARAM_TOMO4F_WIDE = "TOMO4F_WIDE";
    literal System::String^ PARAM_TOMO4F_WIDE_COMMENT = "Defines the Tomo 4F WIDE configuration";
    literal System::String^ PARAM_TOMO4F_WIDE_HOME_DEFAULT = "2550";
    literal System::String^ PARAM_TOMO4F_WIDE_END_DEFAULT = "-2550";
    literal System::String^ PARAM_TOMO4F_WIDE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO4F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO4F_WIDE_SPEED_DEFAULT = "675";
    literal System::String^ PARAM_TOMO4F_WIDE_ACC_DEFAULT = "900";
    literal System::String^ PARAM_TOMO4F_WIDE_DEC_DEFAULT = "900";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_5F Narrow Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_5F Narrow sequence characteristics.
    /// 
    /// < TOMO5F_NARROW, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|825|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-825|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|11|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|150|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|150|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|150|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO5F_NARROW = "TOMO5F_NARROW";
    literal System::String^ PARAM_TOMO5F_NARROW_COMMENT = "Defines the Tomo 5F NARROW configuration";
    literal System::String^ PARAM_TOMO5F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO5F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO5F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO5F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_DEC_DEFAULT = "150";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_5F Intermediate Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_5F Intermediate sequence characteristics.
    /// 
    /// < TOMO5F_INTERMEDIATE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|1300|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-1300|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|13|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|200|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|200|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|200|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE = "TOMO5F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_COMMENT = "Defines the Tomo 5F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT = "200";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO_5F Wide Sequence Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO_5F Wide sequence characteristics.
    /// 
    /// < TOMO5F_WIDE, HomePosition,EndPosition,Fps,Samples,Skip,Speed,Acc,Dec>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |HomePosition|Integer|2609|-|Tube Home position in 0.01 degree unit|
    /// |EndPosition|Integer|-2609|-|Tube End position in 0.01 degree unit|
    /// |Fps|Byte|1|1:6|Sequence Fps|
    /// |Samples|Byte|24|1:255|Number of valid pulses in the sequence|
    /// |Skip|Byte|1|1:255|Number of initial pulses to be discarded (synchronization pulses)|
    /// |Speed|Word|217|-|Speed of the Tube Arm in 0.01 degree/s|
    /// |Acc|Word|217|-|Aceleration of the Tube Arm in 0.01 degree/s^2|
    /// |Dec|Word|217|-|Deceleration of the Tube Arm in 0.01 degree/s^2|
    /// 
    /// 
    literal System::String^ PARAM_TOMO5F_WIDE = "TOMO5F_WIDE";
    literal System::String^ PARAM_TOMO5F_WIDE_COMMENT = "Defines the Tomo 5F WIDE configuration";
    literal System::String^ PARAM_TOMO5F_WIDE_HOME_DEFAULT = "2609";
    literal System::String^ PARAM_TOMO5F_WIDE_END_DEFAULT = "-2609";
    literal System::String^ PARAM_TOMO5F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO5F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SPEED_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_ACC_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_DEC_DEFAULT = "217";


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_TOMO1F_NARROW, PARAM_TOMO1F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_NARROW_HOME_DEFAULT,
            PARAM_TOMO1F_NARROW_END_DEFAULT,
            PARAM_TOMO1F_NARROW_FPS_DEFAULT,
            PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO1F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO1F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO1F_NARROW_ACC_DEFAULT,
            PARAM_TOMO1F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_INTERMEDIATE, PARAM_TOMO1F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_WIDE, PARAM_TOMO1F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_WIDE_HOME_DEFAULT,
            PARAM_TOMO1F_WIDE_END_DEFAULT,
            PARAM_TOMO1F_WIDE_FPS_DEFAULT,
            PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO1F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO1F_WIDE_ACC_DEFAULT,
            PARAM_TOMO1F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_NARROW, PARAM_TOMO2F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_NARROW_HOME_DEFAULT,
            PARAM_TOMO2F_NARROW_END_DEFAULT,
            PARAM_TOMO2F_NARROW_FPS_DEFAULT,
            PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO2F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO2F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO2F_NARROW_ACC_DEFAULT,
            PARAM_TOMO2F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_INTERMEDIATE, PARAM_TOMO2F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_WIDE, PARAM_TOMO2F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_WIDE_HOME_DEFAULT,
            PARAM_TOMO2F_WIDE_END_DEFAULT,
            PARAM_TOMO2F_WIDE_FPS_DEFAULT,
            PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO2F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO2F_WIDE_ACC_DEFAULT,
            PARAM_TOMO2F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_NARROW, PARAM_TOMO3F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_NARROW_HOME_DEFAULT,
            PARAM_TOMO3F_NARROW_END_DEFAULT,
            PARAM_TOMO3F_NARROW_FPS_DEFAULT,
            PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO3F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO3F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO3F_NARROW_ACC_DEFAULT,
            PARAM_TOMO3F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_INTERMEDIATE, PARAM_TOMO3F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_WIDE, PARAM_TOMO3F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_WIDE_HOME_DEFAULT,
            PARAM_TOMO3F_WIDE_END_DEFAULT,
            PARAM_TOMO3F_WIDE_FPS_DEFAULT,
            PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO3F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO3F_WIDE_ACC_DEFAULT,
            PARAM_TOMO3F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_NARROW, PARAM_TOMO4F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_NARROW_HOME_DEFAULT,
            PARAM_TOMO4F_NARROW_END_DEFAULT,
            PARAM_TOMO4F_NARROW_FPS_DEFAULT,
            PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO4F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO4F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO4F_NARROW_ACC_DEFAULT,
            PARAM_TOMO4F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_INTERMEDIATE, PARAM_TOMO4F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_WIDE, PARAM_TOMO4F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_WIDE_HOME_DEFAULT,
            PARAM_TOMO4F_WIDE_END_DEFAULT,
            PARAM_TOMO4F_WIDE_FPS_DEFAULT,
            PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO4F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO4F_WIDE_ACC_DEFAULT,
            PARAM_TOMO4F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_NARROW, PARAM_TOMO5F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_NARROW_HOME_DEFAULT,
            PARAM_TOMO5F_NARROW_END_DEFAULT,
            PARAM_TOMO5F_NARROW_FPS_DEFAULT,
            PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO5F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO5F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO5F_NARROW_ACC_DEFAULT,
            PARAM_TOMO5F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_INTERMEDIATE, PARAM_TOMO5F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_WIDE, PARAM_TOMO5F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_WIDE_HOME_DEFAULT,
            PARAM_TOMO5F_WIDE_END_DEFAULT,
            PARAM_TOMO5F_WIDE_FPS_DEFAULT,
            PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO5F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO5F_WIDE_ACC_DEFAULT,
            PARAM_TOMO5F_WIDE_DEC_DEFAULT
            }),


        }
    );
};
