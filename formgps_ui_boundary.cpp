#include "formgps.h"
#include "qmlutil.h"

void FormGPS::boundary_calculate_area() {
    int ptCount = bnd.bndBeingMadePts.count();
    double area = 0;

    if (ptCount > 0)
    {
        int j = ptCount - 1;  // The last vertex is the 'previous' one to the first

        for (int i = 0; i < ptCount; j = i++)
        {
            area += (bnd.bndBeingMadePts[j].easting + bnd.bndBeingMadePts[i].easting) * (bnd.bndBeingMadePts[j].northing - bnd.bndBeingMadePts[i].northing);
        }
        area = fabs(area / 2);
    }

    qmlItem(qml_root,"boundaryInterface")->setProperty("area", area);
    qmlItem(qml_root,"boundaryInterface")->setProperty("pts", ptCount);
}

void FormGPS::boundary_update_list() {
    QList<QVariant> boundList;
    QMap<QString, QVariant> bndMap;

    int index = 0;

    for (CBoundaryList b: bnd.bndList) {
        bndMap["index"] = index++;
        bndMap["area"] = b.area;
        bndMap["drive_through"] = b.isDriveThru;
        boundList.append(bndMap);
    }
    qmlItem(qml_root,"boundaryInterface")->setProperty("boundary_list", boundList);
}

void FormGPS::boundary_start() {
    bnd.createBndOffset = tool.width * 0.5;
    bnd.isBndBeingMade = true;
    bnd.bndBeingMadePts.clear();
    boundary_calculate_area();
}

void FormGPS::boundary_stop() {
    if (bnd.bndBeingMadePts.count() > 2)
    {
        CBoundaryList New;

        for (int i = 0; i < bnd.bndBeingMadePts.count(); i++)
        {
            New.fenceLine.append(bnd.bndBeingMadePts[i]);
        }

        New.CalculateFenceArea(bnd.bndList.count());
        New.FixFenceLine(bnd.bndList.count());

        bnd.bndList.append(New);
        fd.UpdateFieldBoundaryGUIAreas(bnd.bndList);

        //turn lines made from boundaries
        calculateMinMax();
        FileSaveBoundary();
        bnd.BuildTurnLines(fd);
    }

    //stop it all for adding
    bnd.isOkToAddPoints = false;
    bnd.isBndBeingMade = false;
    bnd.bndBeingMadePts.clear();
    boundary_update_list();
}

void FormGPS::boundary_add_point() {
    bnd.isOkToAddPoints = true;
    AddBoundaryPoint();
    bnd.isOkToAddPoints = false;
}

void FormGPS::boundary_delete_last_point() {
    int ptCount = bnd.bndBeingMadePts.count();
    if (ptCount > 0)
        bnd.bndBeingMadePts.pop_back();
    boundary_calculate_area();
}

void FormGPS::boundary_pause(){
    bnd.isOkToAddPoints = false;
}

void FormGPS::boundary_record() {
    bnd.isOkToAddPoints = true;
}

void FormGPS::boundary_restart() {
    bnd.bndBeingMadePts.clear();
    boundary_calculate_area();
}

void FormGPS::boundary_delete(int which_boundary) {
    //boundary 0 is special.  It's the outer boundary.
    if (which_boundary == 0 && bnd.bndList.count() > 1)
        return; //must remove other boundaries first.

    bnd.bndList.remove(which_boundary);
    boundary_update_list();
}

void FormGPS::boundary_set_drivethru(int which_boundary, bool drive_through) {
    bnd.bndList[which_boundary].isDriveThru = drive_through;
    boundary_update_list();
}

void FormGPS::boundary_delete_all() {
    bnd.bndList.clear();
    FileSaveBoundary();
    bnd.BuildTurnLines(fd);

    boundary_update_list();
}
