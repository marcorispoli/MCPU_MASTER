#pragma once
ref class CanOpenMotor : public System::Windows::Forms::Form
{
public:
	CanOpenMotor();

	void canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len);

private:
	void startupThread(void);
	HWND hwnd;
	unsigned short device_id;

	Thread^ startup_thread;
};

