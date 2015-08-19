//#include "plotWindow.h"
//#include "daq_gui.h"
#include "DAQ_Interface.h"

//#include "themewidget.h"
//#include "plotWindow.h"

//command line format for all DAQs: "DAQ-4W.local|DAQ-4E.local|DAQ-3E.local|DAQ-1W.local|DAQ-1E.local"

int main(int argc, char *argv[])
{



    //create the daq interface object that will be controlling the actions of the daq(s)
    DAQ_Interface* daqInterface = new DAQ_Interface();

    //=======================================================================================================
    //CM 8/6/15 - removing running in gui mode for now, may use in the future when a gui is added
    //if only one argument, then start gui mode
    /*
    if (argc == 1){

        QApplication a(argc, argv);
        PlotWindow window = PlotWindow();
        DAQ_gui daqCollectionInterface = DAQ_gui();

        window.show();
        daqCollectionInterface.show();

        return a.exec();
    }
    //otehrwise, there are command line parameters, so run command line mode
    else if(argc > 1){
    */
    //=======================================================================================================

        //parse out the command line arguments and store in a vector
        std::vector<std::string> allArgs(argv, argv + argc);

        //echo back the command line arguments for validation
        std::vector<std::string>::iterator it;

        if (DEBUG_MODE == 1){
            std::cout << "All Args:" << std::endl;

            for (it = allArgs.begin(); it != allArgs.end(); it++){
                std::cout << *it << std::endl;
            }
        }

        std::cout<<std::endl;

        daqInterface->daqBootStrap(allArgs); //pass the DAQ IDs, and spin off data collection from DAQ(s)

    //} //CM 8/6/15 - commented out to remove gui mode detection

}


