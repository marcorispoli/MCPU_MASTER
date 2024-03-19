
// Calibration Main panel
public: void createCalibrationPanel(void);
public: void initCalibrationPanel(void);
public: void cancCalibrationPanel(void);

// Zero Setting Panel
public: void createZeroSettingPanel(void);
public: void initZeroSettingCalibrationPanel(void);
public: void cancZeroSettingPanel(void);
public: void zeroSettingPanelTimer(void);

	  // Tool Rotation Panel
public: void createRotationToolsPanel(void);
public: void initRotationToolPanel(void);
public: void cancRotationToolPanel(void);
public: void rotationToolPanelTimer(void);
public: void rotationTool_select_item(System::Windows::Forms::PictureBox^ item);
public: void rotationTooCreateInfoPanel(unsigned char reason);
public: void rotationToolSelectionArmOkCallback(void);
public: void rotationToolSelectionTiltOkCallback(void);
public: void rotationToolSelectionBodyOkCallback(void);
public: void rotationToolSelectionSlideOkCallback(void);
public: void rotationToolSelectionCancCallback(void);

// Tool ExposureTool Panel
public: void createExposureToolPanel(void);
public: void initExposureToolPanel(void);
public: void cancExposureToolPanel(void);
public: void ExposureToolPanelTimer(void);
public: void onKvSelectionCallback(System::String^ value);
public: void onmAsSelectionCallback(System::String^ value);
public: bool ExposureToolXrayEna;
public: static void onGeneratorEventLog(System::String^ log);
