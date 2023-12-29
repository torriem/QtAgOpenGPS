#include "formgps.h"


void FormGPS::onBtnManualOffOn_clicked()
{
    qDebug()<<"Manual off on button clicked." ;

    //if Auto is on, turn it off
    autoBtnState = btnStates::Off;
    qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

    switch (manualBtnState)
    {
    case btnStates::Off:
        //roll to "on" state
        manualBtnState = btnStates::On;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",true);
        break;

    case btnStates::On:
        manualBtnState = btnStates::Off;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);
        break;

    }

    //go set the butons and section states
    if (tool.isSectionsNotZones)
        AllSectionsAndButtonsToState(manualBtnState);
    else
        AllZonesAndButtonsToState(manualBtnState);

    manualAllBtnsUpdate();
    closeAllMenus();
}

void FormGPS::onBtnSectionOffAutoOn_clicked(){
    qDebug()<<"Section off auto on button clicked." ;

    //turn off manual if on
    manualBtnState = btnStates::Off;
    qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

    switch (autoBtnState)
    {
    case btnStates::Off:
        autoBtnState = btnStates::Auto;
        qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",true);
        break;

    case btnStates::Auto:
        autoBtnState = btnStates::Off;
        qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);
        break;
    }

    //go set the butons and section states
    if (tool.isSectionsNotZones)
        AllSectionsAndButtonsToState(manualBtnState);
    else
        AllZonesAndButtonsToState(manualBtnState);
    manualAllBtnsUpdate();
    closeAllMenus();
}

//individual buttons for section (called by actual
//qt callback onSectionButton_clicked() SLOT
void FormGPS::onBtnSectionMan_clicked(int sectNumber) {
    if (autoBtnState != btnStates::Auto) {
        //if auto is off just have on-off for choices of section buttons
        if (tool.section[sectNumber].sectionBtnState == btnStates::Off) {
            ///set to auto so that manuBtnUpdate will roll it over to On
            tool.section[sectNumber].sectionBtnState = btnStates::Auto;
        }
    }
    //Roll over button to next state
    manualBtnUpdate(sectNumber);
    if (closeAllMenus()) return;
}

