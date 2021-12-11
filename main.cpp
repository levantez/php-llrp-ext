#include <phpcpp.h>
#include <iostream>
#include "src/app.h"
#include <set>
#include <string>

using namespace std;

class PhpLlrp : public Php::Base 
{
    private:
        char *_host;
        std::string hostString;
        CMyApplication llrpApp;
        CTypeRegistry *pTypeRegistry;
        CConnection *pConn;
        int rc;
        int connected = 0;


    public:
        Php::Value host;

        PhpLlrp() = default;

        virtual ~PhpLlrp() = default;

        void __construct(Php::Parameters &params)
        {
            if (!params.empty()) {
                host = params[0];
                std::string var = params[0];
                hostString = var;
                _host = &hostString[0];
            } 
            connect();
        }

        void connect()
        {
            int status;

            status = llrpApp.justConnect(_host);

            if (status == 0) { 
                connected = 1;
                //printf("Connected...\n");
            }
            else {
                connected = 0;
                if (status == -1) Php::error << "getTheTypeRegistry failed" << std::flush;
                if (status == -2) Php::error << "new CConnection failed" << std::flush;
                if (status == -3) Php::error << "connect error" << std::flush;
                exit(1);
            }
        }

        void checkConnection()
        {
            if (!connected || 0 != llrpApp.checkConnectionStatus()) {
                Php::error << "check llrp connection failed" << std::flush;
                exit(1);
            }
        }

        Php::Value enableImpinjExtensions() {
            checkConnection();
            if (0 != llrpApp.enableImpinjExtensions()) {
                Php::error << "enableImpinjExtensions failed" << std::flush;
                exit(2);
            }
        }
        
        Php::Value resetConfigurationToFactoryDefaults() {
            checkConnection();
            if (0 != llrpApp.resetConfigurationToFactoryDefaults()) {
                Php::error << "resetConfigurationToFactoryDefaults failed" << std::flush;
                exit(3);
            }
        }

        Php::Value writeGpo(Php::Parameters &params) 
        {
            int gpoPort = params[0];
            int state = params[1];
            Php::Value ret = false;

            if (0 == llrpApp.gpoWriteData(gpoPort,state)) ret = true;

            return ret;
        }

        Php::Value getGpoState(Php::Parameters &params) 
        {
            int gpoPort = params[0];
            Php::Value ret;
            ret = llrpApp.gpoGetData(gpoPort);
            return ret;
        }
        
        Php::Value getGpiState(Php::Parameters &params) 
        {
            int gpiPort = params[0];
            Php::Value ret;
            ret = llrpApp.gpiGetData(gpiPort);
            return ret;
        }

        Php::Value getInvent(Php::Parameters &params) 
        {
            //checkConnection();

            std::set<std::string> arr;
            std::set<std::string>::iterator arrIterator;

            int timeout = params[0];

            if (0 != llrpApp.deleteAllROSpec()) {
                Php::warning << "deleteAllROSpec failed" << std::flush;
            }
            if (0 != llrpApp.enableImpinjExtensions()) {
                Php::error << "enableImpinjExtensions failed" << std::flush;
                exit(2);
            } //else printf("enableImpinjExtensions\n");

            // if (0 != llrpApp.resetConfigurationToFactoryDefaults()) {
            //     Php::error << "resetConfigurationToFactoryDefaults failed" << std::flush;
            //     exit(3);
            // } else printf("resetConfigurationToFactoryDefaults\n");

            if(0 != llrpApp.addROSpec()) {
                Php::error << "addROSpec failed" << std::flush;
                exit(4);
            }
            if(0 != llrpApp.enableROSpec()){
                Php::error << "enableROSpec failed" << std::flush;
                exit(5);
            }
            if(0 != llrpApp.startROSpec()) {
                Php::error << "startROSpec failed" << std::flush;
                exit(5);
            }

            llrpApp.awaitAndPrintReportToSetVar(timeout,arr);
                      
            if(0 != llrpApp.stopROSpec()) {
                Php::warning << "stopROSpec failed" << std::flush;
            }

            //arr = llrpApp.runAndGetArr(_host,timeout);

            Php::Value ret;

            if (arr.size()) {
                arrIterator=arr.begin();

                int count = 0;
                while(arrIterator!=arr.end()) {
                    ret[count] = *arrIterator;
                    arrIterator++;
                    count++;
                }
            }

            return ret;
        }

        Php::Value getHost() 
        {
            return host;
        }
};


extern "C" {
    /**
     *  Startup function that is called by the Zend engine 
     *  to retrieve all information about the extension
     *  @return void*
     */
    PHPCPP_EXPORT void *get_module() {
        // create static instance of the extension object
        static Php::Extension myExtension("phpllrp", "1.0");

        // description of the class so that PHP knows which methods are accessible
        Php::Class<PhpLlrp> llrpclass("PhpLlrp");
        llrpclass.method<&PhpLlrp::__construct> ("__construct");
        llrpclass.method<&PhpLlrp::getHost> ("getHost");
        llrpclass.method<&PhpLlrp::getInvent> ("getInvent");
        llrpclass.method<&PhpLlrp::enableImpinjExtensions> ("enableImpinjExtensions");
        llrpclass.method<&PhpLlrp::resetConfigurationToFactoryDefaults> ("resetConfigurationToFactoryDefaults");
        llrpclass.method<&PhpLlrp::writeGpo> ("writeGpo");
        llrpclass.method<&PhpLlrp::getGpoState> ("getGpoState");
        llrpclass.method<&PhpLlrp::getGpiState> ("getGpiState");

        // add the class to the extension
        myExtension.add(std::move(llrpclass));

        // return the extension
        return myExtension;
    }
}