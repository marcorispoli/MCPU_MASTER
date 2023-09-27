#pragma once
#include "TcpIpClientCLI.h"



ref class Generator: public TcpClientCLI
{
public:
	Generator();

	bool isSmartHubConnected(void);
	bool isGeneratorConnected(void);
	bool isGeneratorSetupCompleted(void) { return generator_setup_completed; }
	bool isGeneratorIdle(void) { return generator_idle; }

protected:
	virtual void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;
	Thread^ running_thread;//!< This is the worker thread handler
    void threadWork(void);//!< This is the worker thread for the workflow
	
	bool handleCommandProcessedState(unsigned char* code);
	bool connectionTest(void);

	bool generatorInitialization(void);
	bool generatorSetup(void);
	bool clearSystemMessages(void);
	bool generatorIdle(void);

	bool generator_setup_completed;
	bool generator_idle;
};

