#include "Pythia8/Pythia.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <cmath>

using namespace Pythia8;

int main() {
    // Generator 
    Pythia pythia;
    Event& event = pythia.event;
    // Read in pythia commands from file.
    pythia.readFile("kink.cmnd");

    // Variables Initialization.
    int neta_accepted = 0 ;
    int ndecay1_accepted = 0 ;
    int ndecay2_accepted = 0 ;
    int ndecay3_accepted = 0 ;
    int ndecay4_accepted = 0 ;


    // Initialize.
    pythia.init();
    int stau = 1000015;
    cout << "Lifetime used [mm] =" << scientific << pythia.particleData.tau0(stau) << endl;
    int nEvents = pythia.mode("Main:numberOfEvents");
    //int nAbort = pythia.mode("Main:timesAllowErrors");
    // Histograms
    Hist angel("kink angle", 10.,-180,180);
    Hist length("Decay Length", 100.,0,10000);

    // Begin event loop.
    for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
        if (iEvent%1000 == 0) std::cout << "Event: " << iEvent << std::endl;
        if (!pythia.next()) continue;

        // Loop over all stau decays in the event.
        for (int i = 0; i < event.size(); ++i) {
            int idAbs = event[i].idAbs(); // call particles by ID
            double eta = event[i].eta(); //  eta (pseudorapidity)
            if (idAbs == 1000015) { // if the particle is a stau
                if (eta > 2 && eta < 5) {  // if the particle is in the LHCb eta range
                   neta_accepted++;
                // Find the daughters of the stau.
                   int iDau1 = event[i].daughter1();
                   int iDau2 = event[i].daughter2();
            //cout << "First daughter: " << iDau1 << " Second daughter: " << iDau2 << endl;
            // calcaulte the kink angle if the first daughter is a tau
                   if (event[iDau1].idAbs() == 15) {
                       double kink_angle_rad = event[i].phi() - event[iDau1].phi();
                       double kink_angle = kink_angle_rad * 180.0 / 3.14159;
                       cout << "Kink angle: " << kink_angle << endl;
                          angel.fill(kink_angle);
                   }
            // calcualte the kink angle if the second       
                   if (event[iDau2].idAbs() == 15) {
                       double kink_angle_rad_2 = event[i].phi() - event[iDau2].phi();
                       double kink_angle_2 = kink_angle_rad_2 * 180.0 / 3.14159;
                       cout << "Kink angle: " << kink_angle_2<< endl;
                          angel.fill(kink_angle_2);
                   }                   




            // calculate the decay length inside LHCb.
            //double dist = event[i].vDec().pAbs();
                   double dist = sqrt(pow(event[i].xDec(),2) + pow(event[i].yDec(),2) + pow(event[i].zDec(),2));
                   length.fill(dist);  
                   cout << "Decay length: " << dist << endl;

                // determine if stau decays between 1000 mm and 2000 mm.
                   if (dist > 13000 && dist < 15300) {
                      ndecay1_accepted++;
                    }
                    if (dist > 15000 && dist < 18700) {
                      ndecay2_accepted++;
                    }
                    if (dist > 16300 && dist < 18700) {
                        ndecay3_accepted++;
                    }
                    if (dist > 7800 && dist < 9500) {
                        ndecay4_accepted++;
                    }

                }

}
        }
    }
    // Statistics.
    pythia.stat();
    cout << endl << nEvents << " events generated. " << neta_accepted
    << " events passed eta cut." << endl;
    cout << endl << nEvents << " events generated. " << ndecay1_accepted
    << " events passed between M1-M2 + eta ." << endl;
    cout << endl << nEvents << " events generated. " << ndecay2_accepted
    << " events passed between M2-M5 + eta ." << endl;
    cout << endl << nEvents << " events generated. " << ndecay3_accepted
    << " events passed between M3-M5 + eta ." << endl;
    cout << endl << nEvents << " events generated. " << ndecay4_accepted
    << " events passed between T1-T3 + eta ." << endl;

    HistPlot hpl("DecayAngle");
    hpl.frame( "DecayAnglePlot", "Stau>tau (100 GeV)", "angle (degrees)","Entries");
    hpl.add(angel, "h,red", "KinkAngle");
    hpl.plot();
    HistPlot hpl2("DecayLength");
    hpl2.frame( "DecayLengthPlot", "Stau>tau (100 GeV)", "Length (mm)","Entries");
    hpl2.add(length, "h,blue", "DecayLength");
    hpl2.plot();

    // Done.
    return 0;

}