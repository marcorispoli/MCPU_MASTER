
// Calibration Main panel
public: void createToolsPanel(void);
public: void initToolsPanel(void);
public: void cancToolsPanel(void);

// Rotation Panel
public: void createRotationToolsPanel(void);
public: void initRotationToolPanel(void);
public: void cancRotationToolPanel(void);
public: void rotationToolPanelTimer(void);
public: void rotationTool_select_item(System::Windows::Forms::PictureBox^ item);
public: void rotationToolSelectionArmOkCallback(void); 
public: void rotationToolSelectionTiltOkCallback(void);
public: void rotationToolSelectionBodyOkCallback(void);
public: void rotationToolSelectionSlideOkCallback(void);
public: void rotationToolSelectionCancCallback(void);