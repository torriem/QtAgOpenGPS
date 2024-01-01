#include "formgps.h"
#include <QDir>
#include "aogsettings.h"
#include "cmodulecomm.h"
#include "cboundarylist.h"

QString caseInsensitiveFilename(QString directory, QString filename)
{
    //A bit of a hack to work with files from AOG that might not have
    //the exact case we are expecting. For example, Boundaries.Txt and
    //Headland.Txt (vs txt).

    QStringList search;
    QDir findDir(directory);

    search << filename;

    findDir.setNameFilters(search); //seems to be case insensitive
    if (findDir.count() > 0)
        return findDir[0];
    else
        return filename;

}

void FormGPS::ExportFieldAs_ISOXMLv3()
{
    //TODO use xml library
}

void FormGPS::ExportFieldAs_ISOXMLv4()
{
    //TODO use xml library

}

void FormGPS::FileSaveHeadLines()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "HeadLines.txt");

    QFile headfile(filename);
    if (!headfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&headfile);
    writer.setLocale(QLocale::C);

    int cnt = hdl.tracksArr.count();

    if (cnt > 0)
    {
        writer << "$HeadLines" << Qt::endl;

        for (int i = 0; i < cnt; i++)
        {
            //write out the name
            writer << hdl.tracksArr[i].name << Qt::endl;


            //write out the moveDistance
            writer << hdl.tracksArr[i].moveDistance << Qt::endl;

            //write out the mode
            writer << hdl.tracksArr[i].mode << Qt::endl;

            //write out the A_Point index
            writer << hdl.tracksArr[i].a_point << Qt::endl;

            //write out the points of ref line
            int cnt2 = hdl.tracksArr[i].trackPts.count();

            writer << cnt2 << Qt::endl;
            if (hdl.tracksArr[i].trackPts.count() > 0)
            {
                for (int j = 0; j < cnt2; j++)
                    writer << qSetRealNumberPrecision(3) << hdl.tracksArr[i].trackPts[j].easting <<
                        "," << qSetRealNumberPrecision(3) << hdl.tracksArr[i].trackPts[j].northing <<
                        "," << qSetRealNumberPrecision(5) << hdl.tracksArr[i].trackPts[j].heading << Qt::endl;
            }
        }
    }
    else
    {
        writer << "$HeadLines" << Qt::endl;
        return;
    }

    if (hdl.idx > (hdl.tracksArr.count() - 1)) hdl.idx = hdl.tracksArr.count() - 1;
}

void FormGPS::FileLoadHeadLines()
{
    //current field directory should already exist
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "HeadLines.txt");

    QFile headfile(filename);
    if (!headfile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&headfile);
    reader.setLocale(QLocale::C);

    hdl.tracksArr.clear();

    //get the file of previous AB Lines
    QString line;

    //read header $HeadLies
    line = reader.readLine();

    while (!reader.atEnd())
    {

        hdl.tracksArr.append(CHeadPath());
        hdl.idx = hdl.tracksArr.count() - 1;

        hdl.tracksArr[hdl.idx].name = reader.readLine();

        line = reader.readLine();
        hdl.tracksArr[hdl.idx].moveDistance = line.toDouble();

        line = reader.readLine();
        hdl.tracksArr[hdl.idx].mode = line.toInt();

        line = reader.readLine();
        hdl.tracksArr[hdl.idx].a_point = line.toInt();

        line = reader.readLine();
        int numPoints = (line.toInt());

        if (numPoints > 3)
        {
            hdl.tracksArr[hdl.idx].trackPts.clear();

            for (int i = 0; i < numPoints; i++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                Vec3 vecPt(words[0].toDouble(),
                           words[1].toDouble(),
                           words[2].toDouble());
                hdl.tracksArr[hdl.idx].trackPts.append(vecPt);
            }
        }
        else
        {
            if (hdl.tracksArr.count() > 0)
            {
                hdl.tracksArr.removeAt(hdl.idx);
            }
        }
    }

    hdl.idx = -1;
}

void FormGPS::FileSaveCurveLines()
{
    curve.moveDistance = 0;

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "CurveLines.txt");

    int cnt = curve.curveArr.count();
    curve.numCurveLines = cnt;

    QFile curveFile(filename);
    if (!curveFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&curveFile);
    writer.setLocale(QLocale::C);

    writer << "$CurveLines" << Qt::endl;

    if (cnt > 0)
    {
        for (int i = 0; i < cnt; i++)
        {
            //write out the Name
            writer << curve.curveArr[i].Name << Qt::endl;

            //write out the aveheading
            writer << curve.curveArr[i].aveHeading << Qt::endl;

            //write out the points of ref line
            int cnt2 = curve.curveArr[i].curvePts.count();

            writer << cnt2 << Qt::endl;
            if (curve.curveArr[i].curvePts.count() > 0)
            {
                for (int j = 0; j < cnt2; j++)
                    writer << qSetRealNumberPrecision(3) << curve.curveArr[i].curvePts[j].easting << ","
                           << qSetRealNumberPrecision(3) << curve.curveArr[i].curvePts[j].northing << ","
                           << qSetRealNumberPrecision(5) << curve.curveArr[i].curvePts[j].heading << Qt::endl;
            }
        }
    }
    else
    {
        writer << "$CurveLines" << Qt::endl;
    }

    if (curve.numCurveLines == 0) curve.numCurveLineSelected = 0;
    if (curve.numCurveLineSelected > curve.numCurveLines) curve.numCurveLineSelected = curve.numCurveLines;

    curveFile.close();
}

void FormGPS::FileLoadCurveLines()
{
    curve.moveDistance = 0;
    curve.curveArr.clear();
    curve.numCurveLines = 0;

    //current field directory should already exist
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "CurveLines.txt");

    QFile curveFile(filename);
    if (!curveFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&curveFile);
    reader.setLocale(QLocale::C);

    QString line;

    //read header $CurveLine
    line = reader.readLine();

    while (!reader.atEnd())
    {
        line = reader.readLine();
        if(line.isNull()) break; //no more to read

        curve.curveArr.append(CCurveLines());

        //read header $CurveLine
        curve.curveArr[curve.numCurveLines].Name = line;
        // get the average heading
        line = reader.readLine();
        curve.curveArr[curve.numCurveLines].aveHeading = line.toDouble();

        line = reader.readLine();
        int numPoints = line.toInt();

        if (numPoints > 1)
        {
            curve.curveArr[curve.numCurveLines].curvePts.clear();

            for (int i = 0; i < numPoints; i++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                Vec3 vecPt(words[0].toDouble(),
                           words[1].toDouble(),
                           words[2].toDouble());
                curve.curveArr[curve.numCurveLines].curvePts.append(vecPt);
            }
            curve.numCurveLines++;
        }
        else
        {
            if (curve.curveArr.count() > 0)
            {
                curve.curveArr.removeAt(curve.numCurveLines);
            }
        }
    }

    if (curve.numCurveLines == 0) curve.numCurveLineSelected = 0;
    if (curve.numCurveLineSelected > curve.numCurveLines) curve.numCurveLineSelected = curve.numCurveLines;

    curveFile.close();
}

void FormGPS::FileSaveABLines()
{
    ABLine.moveDistance = 0;

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "ABLines.txt" );

    QFile lineFile(filename);
    if (!lineFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&lineFile);
    writer.setLocale(QLocale::C);

    int cnt = ABLine.lineArr.count();

    if (cnt > 0)
    {
        foreach (CABLines item, ABLine.lineArr)
        {
            //make it culture invariant
            writer << item.Name << ","
                   << qSetRealNumberPrecision(8) << glm::toDegrees(item.heading) << ","
                   << qSetRealNumberPrecision(3) << item.origin.easting << ","
                   << qSetRealNumberPrecision(3) << item.origin.northing << Qt::endl;
        }
    }

    if (ABLine.numABLines == 0) ABLine.numABLineSelected = 0;
    if (ABLine.numABLineSelected > ABLine.numABLines) ABLine.numABLineSelected = ABLine.numABLines;

    lineFile.close();
}

void FormGPS::FileLoadABLines()
{
    ABLine.moveDistance = 0;

    //current field directory should already exist
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "ABLines.txt");

    QFile linesFile(filename);
    if (!linesFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&linesFile);
    reader.setLocale(QLocale::C);

    QString line;
    ABLine.numABLines = 0;
    ABLine.numABLineSelected = 0;
    ABLine.lineArr.clear();

    //read all the lines
    for (int i = 0; !reader.atEnd(); i++)
    {

        line = reader.readLine();
        QStringList words = line.split(',');

        if (words.length() != 4) break;

        ABLine.lineArr.append(CABLines());

        ABLine.lineArr[i].Name = words[0];


        ABLine.lineArr[i].heading = glm::toRadians(words[1].toDouble());
        ABLine.lineArr[i].origin.easting = words[2].toDouble();
        ABLine.lineArr[i].origin.northing = words[3].toDouble();
        ABLine.numABLines++;
    }

    if (ABLine.numABLines == 0) ABLine.numABLineSelected = 0;
    if (ABLine.numABLineSelected > ABLine.numABLines) ABLine.numABLineSelected = ABLine.numABLines;

    linesFile.close();
}

bool FormGPS::FileOpenField(QString fieldDir)
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + fieldDir;

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Field.txt");

    QFile fieldFile(filename);
    if (!fieldFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open field " << filename << "for reading!";
        //TODO timed messagebox
        return false;
    }

    QTextStream reader(&fieldFile);
    reader.setLocale(QLocale::C);

    //close the existing job and reset everything
    JobClose();

    //and open a new job
    jobNew();

    //Saturday, February 11, 2017  -->  7:26:52 AM
    //$FieldDir
    //Bob_Feb11
    //$Offsets
    //533172,5927719,12 - offset easting, northing, zone

    //start to read the file
    QString line;

    //Date time line
    line = reader.readLine();

    //dir header $FieldDir
    line = reader.readLine();

    //read field directory
    line = reader.readLine();

    currentFieldDirectory = line.trimmed();

    //Offset header
    line = reader.readLine();

    //read the Offsets
    line = reader.readLine();
    QStringList offs = line.split(',');
    //pn.utmEast = offs[0].toInt();
    //pn.utmNorth = offs[1].toInt();
    //pn.actualEasting = offs[0].toDouble();
    //pn.actualNorthing = offs[1].toDouble();
    //pn.zone = offs[2].toInt();
    //isFirstFixPositionSet = true;

    //convergence angle update
    if (!reader.atEnd())
    {
        line = reader.readLine(); //Convergence
        line = reader.readLine();
        //pn.convergenceAngle = line.toDouble();
        //TODO lblConvergenceAngle.Text = Math.Round(glm.toDegrees(pn.convergenceAngle), 3).ToString();
    }

    //start positions
    if (!reader.atEnd())
    {
        line = reader.readLine(); //eat StartFix
        line = reader.readLine();
        offs = line.split(',');

        pn.latStart = offs[0].toDouble();
        pn.lonStart = offs[1].toDouble();

        if (timerSim.isActive())
        {
            pn.latitude = pn.latStart;
            pn.longitude = pn.lonStart;

            sim.latitude = pn.latStart;
            sim.longitude = pn.lonStart;
        }
        pn.SetLocalMetersPerDegree();
    }

    fieldFile.close();


    // ABLine -------------------------------------------------------------------------------------------------
    FileLoadABLines();

    if (ABLine.lineArr.count() > 0)
    {
        ABLine.numABLineSelected = 1;
        ABLine.refPoint1 = ABLine.lineArr[ABLine.numABLineSelected - 1].origin;
        //ABLine.refPoint2 = ABLine.lineArr[ABLine.numABLineSelected - 1].ref2;
        ABLine.abHeading = ABLine.lineArr[ABLine.numABLineSelected - 1].heading;
        ABLine.SetABLineByHeading(ABLine.abHeading);
        //ABLine.isABLineSet = false;
        ABLine.isABLineSet = false;
        ABLine.isABLineLoaded = true;
    }
    else
    {
        ABLine.isABLineSet = false;
        ABLine.isABLineLoaded = false;
    }


    //CurveLines
    FileLoadCurveLines();
    if (curve.curveArr.count() > 0)
    {
        curve.numCurveLineSelected = 1;
        int idx = curve.numCurveLineSelected - 1;
        curve.aveLineHeading = curve.curveArr[idx].aveHeading;

        curve.refList.clear();
        for (int i = 0; i < curve.curveArr[idx].curvePts.count(); i++)
        {
            curve.refList.append(curve.curveArr[idx].curvePts[i]);
        }
        curve.isCurveSet = true;
    }
    else
    {
        curve.isCurveSet = false;
        curve.refList.clear();
    }

    //section patches
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Sections.txt");

    QFile sectionsFile(filename);
    if (!sectionsFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open sections " << filename << "for reading!";
        //TODO timed messagebox
        //return;
    } else
    {

        reader.setDevice(&sectionsFile);
        bool isv3 = false;
        fd.distanceUser = 0;
        QVector3D vecFix;

        //read header
        while (!reader.atEnd())
        {
            line = reader.readLine();
            if (line.contains("ect"))
            {
                isv3 = true;
                break;
            }

            int verts = line.toInt();

            triStrip[0].triangleList = QSharedPointer<PatchTriangleList>( new PatchTriangleList);
            triStrip[0].patchList.append(triStrip[0].triangleList);


            for (int v = 0; v < verts; v++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                vecFix.setX(words[0].toDouble());
                vecFix.setY(words[1].toDouble());
                vecFix.setZ(words[2].toDouble());
                triStrip[0].triangleList->append(vecFix);
            }

            //calculate area of this patch - AbsoluteValue of (Ax(By-Cy) + Bx(Cy-Ay) + Cx(Ay-By)/2)
            verts -= 2;
            if (verts >= 2)
            {
                for (int j = 1; j < verts; j++)
                {
                    double temp = 0;
                    temp = (*triStrip[0].triangleList)[j].x() * ((*triStrip[0].triangleList)[j + 1].y() - (*triStrip[0].triangleList)[j + 2].y()) +
                             (*triStrip[0].triangleList)[j + 1].x() * ((*triStrip[0].triangleList)[j + 2].y() - (*triStrip[0].triangleList)[j].y()) +
                                 (*triStrip[0].triangleList)[j + 2].x() * ((*triStrip[0].triangleList)[j].y() - (*triStrip[0].triangleList)[j + 1].y());

                    fd.workedAreaTotal += fabs((temp * 0.5));
                }
            }

            //was old version prior to v4
            if (isv3)
            {
                    //Append the current list to the field file
            }
        }
        sectionsFile.close();
    }

    // Contour points ----------------------------------------------------------------------------

    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Contour.txt");

    QFile contourFile(filename);
    if (!contourFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open contour " << filename << "for reading!";
        //TODO timed messagebox
    } else
    {

        reader.setDevice(&contourFile);

        //read header
        line = reader.readLine();

        while (!reader.atEnd())
        {
            //read how many vertices in the following patch
            line = reader.readLine();
            int verts = line.toInt();

            Vec3 vecFix(0, 0, 0);

            ct.ptList = QSharedPointer<QVector<Vec3>>(new QVector<Vec3>());
            ct.stripList.append(ct.ptList);

            for (int v = 0; v < verts; v++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                vecFix.easting = words[0].toDouble();
                vecFix.northing = words[1].toDouble();
                vecFix.heading = words[2].toDouble();
                ct.ptList->append(vecFix);
            }
        }

        contourFile.close();
    }

    // Flags -------------------------------------------------------------------------------------------------

    //Either exit or update running save
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Flags.txt");

    QFile flagsFile(filename);
    if (!flagsFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open flags " << filename << "for reading!";
        //TODO timed messagebox
        //return;
    } else
    {

        reader.setDevice(&flagsFile);

        flagPts.clear();
        //read header
        line = reader.readLine();

        //number of flags
        line = reader.readLine();
        int points = line.toInt();

        if (points > 0)
        {
            double lat;
            double longi;
            double east;
            double nort;
            double head;
            int color, ID;
            QString notes;

            for (int v = 0; v < points; v++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');

                if (words.count() == 8)
                {
                    lat = words[0].toDouble();
                    longi = words[1].toDouble();
                    east = words[2].toDouble();
                    nort = words[3].toDouble();
                    head = words[4].toDouble();
                    color = words[5].toInt();
                    ID = words[6].toInt();
                    notes = words[7].trimmed();
                }
                else
                {
                    lat = words[0].toDouble();
                    longi = words[1].toDouble();
                    east = words[2].toDouble();
                    nort = words[3].toDouble();
                    head = 0;
                    color = words[4].toInt();
                    ID = words[5].toInt();
                    notes = "";
                }

                CFlag flagPt(lat, longi, east, nort, head, color, ID, notes);
                flagPts.append(flagPt);
            }
        }
        flagsFile.close();

    }

    //Boundaries
    //Either exit or update running save
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Boundary.txt");

    QFile boundariesFile(filename);
    if (!boundariesFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open boundaries " << filename << "for reading!";
        //TODO timed messagebox
        //return;
    } else
    {

        reader.setDevice(&boundariesFile);
        //read header
        line = reader.readLine();//Boundary

        for (int k = 0; true; k++)
        {
            if (reader.atEnd()) break;
            CBoundaryList New;

            //True or False OR points from older boundary files
            line = reader.readLine();

            //Check for older boundary files, then above line string is num of points
            if (line == "True")
            {
                New.isDriveThru = true;
                line = reader.readLine();
            } else if (line == "False")
            {
                New.isDriveThru = false;
                line = reader.readLine(); //number of points
            }

            //Check for latest boundary files, then above line string is num of points
            if (line == "True" || line == "False")
            {
               line = reader.readLine(); //number of points
            }

            int numPoints = line.toInt();

            if (numPoints > 0)
            {
                //load the line
                for (int i = 0; i < numPoints; i++)
                {
                    line = reader.readLine();
                    QStringList words = line.split(',');
                    Vec3 vecPt( words[0].toDouble(),
                                words[1].toDouble(),
                                words[2].toDouble() );

                    //if (turnheading)
                    //{
                    //    vecPt.heading = vecPt.heading + Math.PI;
                    //}
                    New.fenceLine.append(vecPt);
                }

                New.CalculateFenceArea(k);

                double delta = 0;
                New.fenceLineEar.clear();

                for (int i = 0; i < New.fenceLine.count(); i++)
                {
                    if (i == 0)
                    {
                        New.fenceLineEar.append(Vec2(New.fenceLine[i].easting, New.fenceLine[i].northing));
                        continue;
                    }
                    delta += (New.fenceLine[i - 1].heading - New.fenceLine[i].heading);
                    if (fabs(delta) > 0.005)
                    {
                        New.fenceLineEar.append(Vec2(New.fenceLine[i].easting, New.fenceLine[i].northing));
                        delta = 0;
                    }
                }
                bnd.bndList.append(New);
            }
        }
        calculateMinMax();
        bnd.BuildTurnLines(fd);

        if(bnd.bndList.count() > 0)
        {
            //TODO: inform GUI btnABDraw can be seen
        }
        boundariesFile.close();
    }
    // Headland  -------------------------------------------------------------------------------------------------
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Headland.txt");

    QFile headlandFile(filename);
    if (!headlandFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open headland " << filename << "for reading!";
        //TODO timed messagebox
    } else {
        reader.setDevice(&headlandFile);

        //read header
        line = reader.readLine();

        for (int k = 0; true; k++)
        {
            if (reader.atEnd()) break;

            if (bnd.bndList.count() > k)
            {
                bnd.bndList[k].hdLine.clear();

                //read the number of points
                line = reader.readLine();
                int numPoints = line.toInt();

                if (numPoints > 0)
                {
                    //load the line
                    for (int i = 0; i < numPoints; i++)
                    {
                        line = reader.readLine();
                        QStringList words = line.split(',');
                        Vec3 vecPt(words[0].toDouble(),
                                   words[1].toDouble(),
                                   words[2].toDouble());
                        bnd.bndList[k].hdLine.append(vecPt);
                    }
                }
            }
        }
    }

    if (bnd.bndList.count() > 0 && bnd.bndList[0].hdLine.count() > 0)
    {
        bnd.isHeadlandOn = true;
        //TODO: tell GUI to enable headlands
        //btnHeadlandOnOff.Image = Properties.Resources.HeadlandOn;
        //btnHeadlandOnOff.Visible = true;
        //btnHydLift.Visible = true;
        //btnHydLift.Image = Properties.Resources.HydraulicLiftOff;

    }
    else
    {
        bnd.isHeadlandOn = false;
        //TODO: tell GUI
        //btnHeadlandOnOff.Image = Properties.Resources.HeadlandOff;
        //btnHeadlandOnOff.Visible = false;
        //btnHydLift.Visible = false;
    }

    //trams ---------------------------------------------------------------------------------
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Tram.txt");

    tram.tramBndOuterArr.clear();
    tram.tramBndInnerArr.clear();
    tram.tramList.clear();
    tram.displayMode = 0;
    //btnTramDisplayMode.Visible = false;

    QFile tramFile(filename);
    if (!tramFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open headland " << filename << "for reading!";
        //TODO timed messagebox
    } else {
        reader.setDevice(&tramFile);
            //read header
        line = reader.readLine();//$Tram

        //outer track of boundary tram
        line = reader.readLine();
        if (!line.isNull())
        {
            int numPoints = line.toInt();

            if (numPoints > 0)
            {
                //load the line
                for (int i = 0; i < numPoints; i++)
                {
                    line = reader.readLine();
                    QStringList words = line.split(',');
                    Vec2 vecPt(
                        words[0].toDouble(),
                        words[1].toDouble());

                    tram.tramBndOuterArr.append(vecPt);
                }
                tram.displayMode = 1;
            }

            //inner track of boundary tram
            line = reader.readLine();
            numPoints = line.toInt();

            if (numPoints > 0)
            {
                //load the line
                for (int i = 0; i < numPoints; i++)
                {
                    line = reader.readLine();
                    QStringList words = line.split(',');
                    Vec2 vecPt(
                        words[0].toDouble(),
                        words[1].toDouble());

                    tram.tramBndInnerArr.append(vecPt);
                }
            }

            if (!reader.atEnd())
            {
                line = reader.readLine();
                int numLines = line.toInt();

                for (int k = 0; k < numLines; k++)
                {
                    line = reader.readLine();
                    numPoints = line.toInt();

                    tram.tramArr = QSharedPointer<QVector<Vec2>>(new QVector<Vec2>);
                    tram.tramList.append(tram.tramArr);

                    for (int i = 0; i < numPoints; i++)
                    {
                        line = reader.readLine();
                        QStringList words = line.split(',');
                        Vec2 vecPt(
                            words[0].toDouble(),
                            words[1].toDouble());

                        tram.tramArr->append(vecPt);
                    }
                }
            }
        }

        FixTramModeButton();
    }

    FixPanelsAndMenus();
    SetZoom();



    //Recorded Path
    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "RecPath.txt");

    QFile recpathFile(filename);
    if (!recpathFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open recpath " << filename << "for reading!";
        //TODO timed messagebox
    } else
    {

        reader.setDevice(&recpathFile);
        //read header
        line = reader.readLine();
        line = reader.readLine();
        int numPoints = line.toInt();
        recPath.recList.clear();

        while (!reader.atEnd())
        {
            for (int v = 0; v < numPoints; v++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                CRecPathPt point(
                    words[0].toDouble(),
                    words[1].toDouble(),
                    words[2].toDouble(),
                    words[3].toDouble(),
                    (words[4] == "True" ? true : false) );

                //add the point
                recPath.recList.append(point);
            }
        }

        if (recPath.recList.count() > 0)
        {
            //TODO: panelDrag.Visible = true;
        } else {
            //TODO: panelDrag.Visible = false;
        }
    }

    worldGrid.isGeoMap = false;

    filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "BackPic.txt");

    QFile backPic(filename);
    if (backPic.open(QIODevice::ReadOnly))
    {
        reader.setDevice(&backPic);

        //read header
        line = reader.readLine();

        line = reader.readLine();
        worldGrid.isGeoMap = (line == "True" ? true : false);

        line = reader.readLine();
        worldGrid.eastingMaxGeo = line.toDouble();
        line = reader.readLine();
        worldGrid.eastingMinGeo = line.toDouble();
        line = reader.readLine();
        worldGrid.northingMaxGeo = line.toDouble();
        line = reader.readLine();
        worldGrid.northingMinGeo = line.toDouble();


        if (worldGrid.isGeoMap)
        {
            //TODO: load map texture
            worldGrid.isGeoMap = false;
        }
        //Refresh GL view

    }

    return true;
}

void FormGPS::FileCreateField()
{
    //Saturday, February 11, 2017  -->  7:26:52 AM
    //$FieldDir
    //Bob_Feb11
    //$Offsets
    //533172,5927719,12 - offset easting, northing, zone

    if( ! isJobStarted)
    {
        qDebug() << "field not open";
        TimedMessageBox(3000, tr("Field Not Open"), tr("Create a new field."));
        return;
    }

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Field.txt");
    QFile fieldFile(myFilename);
    if (!fieldFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for writing!";
        return;
    }

    QTextStream writer(&fieldFile);
    writer.setLocale(QLocale::C);

    QDateTime now = QDateTime::currentDateTime();

    //Write out the date
    writer << now.toString("yyyy-MMMM-dd hh:mm:ss tt") << Qt::endl;

    writer << "$FieldDir" << Qt::endl;
    writer << currentFieldDirectory << Qt::endl;

    //write out the easting and northing Offsets
    writer << "$Offsets" << Qt::endl;
    writer << "0,0" << Qt::endl;

    writer << "Convergence" << Qt::endl;
    writer << "0" << Qt::endl;

    writer << "StartFix" << Qt::endl;
    writer << pn.latitude << "," << pn.longitude << Qt::endl;

    fieldFile.close();
}

void FormGPS::FileCreateElevation()
{
    //Saturday, February 11, 2017  -->  7:26:52 AM
    //$FieldDir
    //Bob_Feb11
    //$Offsets
    //533172,5927719,12 - offset easting, northing, zone

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Elevation.txt");
    QFile fieldFile(myFilename);
    if (!fieldFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for writing!";
        return;
    }

    QTextStream writer(&fieldFile);
    writer.setLocale(QLocale::C);

    QDateTime now = QDateTime::currentDateTime();

    //Write out the date
    writer << now.toString("yyyy-MMMM-dd hh:mm:ss tt") << Qt::endl;

    writer << "$FieldDir" << Qt::endl;
    writer << currentFieldDirectory << Qt::endl;

    //write out the easting and northing Offsets
    writer << "$Offsets" << Qt::endl;
    writer << "0,0" << Qt::endl;

    writer << "Convergence" << Qt::endl;
    writer << "0" << Qt::endl;

    writer << "StartFix" << Qt::endl;
    writer << pn.latitude << "," << pn.longitude << Qt::endl;

    fieldFile.close();
}

//save field Patches
void FormGPS::FileSaveSections()
{
    if (tool.patchSaveList.count() == 0) return;

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Sections.txt");
    QFile sectionFile(myFilename);
    if (!sectionFile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }

    QTextStream writer(&sectionFile);
    writer.setLocale(QLocale::C);

    //for each patch, write out the list of triangles to the file
    QSharedPointer<QVector<QVector3D>> triList;
    foreach(triList, tool.patchSaveList)
    {
        int count2 = triList->count();
        writer << count2 << Qt::endl;

        for (int i=0; i < count2; i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << (*triList)[i].x() << "," << (*triList)[i].y()
                   << "," << (*triList)[i].z() << Qt::endl;
        }
    }

    //clear out that patchList and begin adding new ones for next save
    tool.patchSaveList.clear();
    sectionFile.close();
}

void FormGPS::FileCreateSections()
{
    //FileSaveSections appends; we must create the file, overwriting any existing vesion
    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Sections.txt");
    QFile sectionFile(myFilename);
    if (!sectionFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }
    //file should now exist; we can close it.
    sectionFile.close();

}

void FormGPS::FileCreateBoundary()
{
    //Create Boundary.txt, overwriting it if it exists.
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Boundary.txt");

    QFile boundfile(filename);
    if (!boundfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&boundfile);
    writer.setLocale(QLocale::C);
    writer << "$Boundary" << Qt::endl;
}

void FormGPS::FileCreateFlags()
{
    //create a new flags file, overwriting if it alraedy existis.
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Flags.txt");

    QFile flagsFile(filename);
    if (!flagsFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }
}

void FormGPS::FileCreateContour()
{
    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Contour.txt");
    QFile contourFile(myFilename);
    if (!contourFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }

    QTextStream writer(&contourFile);
    writer.setLocale(QLocale::C);

    writer << "$Contour" << Qt::endl;
}

void FormGPS::FileSaveContour()
{
    if (contourSaveList.count() == 0) return;

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Contour.txt");
    QFile contourFile(myFilename);
    if (!contourFile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }

    QTextStream writer(&contourFile);
    writer.setLocale(QLocale::C);
    QSharedPointer<QVector<Vec3>> triList;

    foreach (triList, contourSaveList)
    {
        int count2 = triList->count();

        writer << count2 << Qt::endl;

        for (int i = 0; i < count2; i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << (*triList)[i].easting << ","
                   << (*triList)[i].northing << ","
                   << (*triList)[i].heading << Qt::endl;
        }
    }

    contourSaveList.clear();
    contourFile.close();
}

void FormGPS::FileSaveBoundary()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Boundary.txt");

    QFile boundfile(filename);
    if (!boundfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&boundfile);
    writer.setLocale(QLocale::C);
    writer << "$Boundary" << Qt::endl;
    for(int i = 0; i < bnd.bndList.count(); i++)
    {
        writer << (bnd.bndList[i].isDriveThru ? "True" : "False") << Qt::endl;

        writer << bnd.bndList[i].fenceLine.count() << Qt::endl;
        if (bnd.bndList[i].fenceLine.count() > 0)
        {
            for (int j = 0; j < bnd.bndList[i].fenceLine.count(); j++)
                writer << qSetRealNumberPrecision(3)
                       << bnd.bndList[i].fenceLine[j].easting << ","
                       << bnd.bndList[i].fenceLine[j].northing << ","
                       << qSetRealNumberPrecision(5)
                       << bnd.bndList[i].fenceLine[j].heading << Qt::endl;
        }
    }

    boundfile.close();

}

void FormGPS::FileSaveTram()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Tram.txt");

    QFile tramFile(filename);
    if (!tramFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&tramFile);
    writer.setLocale(QLocale::C);

    writer << "$Tram" << Qt::endl;

    if (tram.tramBndOuterArr.count() > 0)
    {
        //outer track of outer boundary tram
        writer << tram.tramBndOuterArr.count() << Qt::endl;

        for (int i = 0; i < tram.tramBndOuterArr.count(); i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << tram.tramBndOuterArr[i].easting << ","
                   << tram.tramBndOuterArr[i].northing << Qt::endl;
        }

        //inner track of outer boundary tram
        writer << tram.tramBndInnerArr.count();

        for (int i = 0; i < tram.tramBndInnerArr.count(); i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << tram.tramBndInnerArr[i].easting << ","
                   << tram.tramBndInnerArr[i].northing << Qt::endl;
        }
    }

    //no outer bnd
    else
    {
        writer << "0" << Qt::endl;
        writer << "0" << Qt::endl;
    }

    if (tram.tramList.count() > 0)
    {
        writer << tram.tramList.count() << Qt::endl;
        for (int i = 0; i < tram.tramList.count(); i++)
        {
            writer << tram.tramList[i]->count() << Qt::endl;

            for (int h = 0; h < tram.tramList[i]->count(); h++)
            {
            writer << qSetRealNumberPrecision(3)
                       << (*tram.tramList[i])[h].easting << ","
                       << (*tram.tramList[i])[h].northing << Qt::endl;
            }
        }
    }
}

void FormGPS::FileSaveBackPic()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "BackPic.txt");

    QFile backFile(filename);
    if (!backFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&backFile);
    writer.setLocale(QLocale::C);

    writer << "$BackPic" << Qt::endl;

    if (worldGrid.isGeoMap)
    {
        writer << "True" << Qt::endl;
        writer << worldGrid.eastingMaxGeo << Qt::endl;
        writer << worldGrid.eastingMinGeo << Qt::endl;
        writer << worldGrid.northingMaxGeo << Qt::endl;
        writer << worldGrid.northingMinGeo << Qt::endl;
    }
    else
    {
        writer << "False" << Qt::endl;
        writer << 300 << Qt::endl;
        writer << -300 << Qt::endl;
        writer << 300 << Qt::endl;
        writer << -300 << Qt::endl;
    }
}

void FormGPS::FileSaveHeadland()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Headland.txt");

    QFile headfile(filename);
    if (!headfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&headfile);
    writer.setLocale(QLocale::C);
    writer << "$Headland" << Qt::endl;
    if (bnd.bndList.count() > 0 && bnd.bndList[0].hdLine.count() > 0)
    {
        for(int i = 0; i < bnd.bndList.count(); i++)
        {
            writer << bnd.bndList[i].hdLine.count() << Qt::endl;
            if (bnd.bndList[i].hdLine.count() > 0)
            {
                for (int j = 0; j < bnd.bndList[i].hdLine.count(); j++)
                    writer << qSetRealNumberPrecision(3)
                           << bnd.bndList[i].hdLine[j].easting << ","
                           << bnd.bndList[i].hdLine[j].northing << ","
                           << bnd.bndList[i].hdLine[j].heading << Qt::endl;
            }
        }
    }

    headfile.close();

}

void FormGPS::FileCreateRecPath()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "RecPath.txt");

    QFile recpathfile(filename);
    if (!recpathfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&recpathfile);
    writer.setLocale(QLocale::C);

    writer << "$RecPath" << Qt::endl;
    writer << "0" << Qt::endl;

    recpathfile.close();

}

void FormGPS::FileSaveRecPath()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "RecPath.txt");

    QFile recpathfile(filename);
    if (!recpathfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&recpathfile);
    writer.setLocale(QLocale::C);

    writer << "$RecPath" << Qt::endl;
    writer << recPath.recList.count() << Qt::endl;

    if (recPath.recList.count() > 0)
    {
        for (int j = 0; j < recPath.recList.count(); j++)
            writer << qSetRealNumberPrecision(3)
                   << recPath.recList[j].easting << ","
                   << recPath.recList[j].northing << ","
                   << recPath.recList[j].heading << ","
                   << qSetRealNumberPrecision(1)
                   << recPath.recList[j].speed << ","
                   << recPath.recList[j].autoBtnState << Qt::endl;

    }

    recpathfile.close();

}

void FormGPS::FileLoadRecPath()
{
    //current field directory should already exist
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "RecPath.txt");

    QFile recFile(filename);
    if (!recFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&recFile);
    reader.setLocale(QLocale::C);

    //read header
    QString line = reader.readLine();
    line = reader.readLine();
    int numPoints = line.toInt();
    recPath.recList.clear();

    while (!reader.atEnd())
    {
        for (int v = 0; v < numPoints; v++)
        {
            line = reader.readLine();
            QStringList words = line.split(',');
            CRecPathPt point(
                words[0].toDouble(),
                words[1].toDouble(),
                words[2].toDouble(),
                words[3].toDouble(),
                (words[4] == "True" ? true : false));

            //add the point
            recPath.recList.append(point);
        }
    }
}

void FormGPS::FileSaveFlags()
{
    //Saturday, February 11, 2017  -->  7:26:52 AM
    //$FlagsDir
    //Bob_Feb11
    //$Offsets
    //533172,5927719,12 - offset easting, northing, zone

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Flags.txt");

    QFile flagsfile(filename);
    if (!flagsfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&flagsfile);
    writer.setLocale(QLocale::C);

    writer << "$Flags" << Qt::endl;

    int count2 = flagPts.count();
    writer << count2 << Qt::endl;

    for (int i = 0; i < count2; i++)
    {
        writer << flagPts[i].latitude << ","
               << flagPts[i].longitude << ","
               << flagPts[i].easting << ","
               << flagPts[i].northing << ","
               << flagPts[i].heading << ","
               << flagPts[i].color << ","
               << flagPts[i].ID << ","
               << flagPts[i].notes << Qt::endl;
    }

    flagsfile.close();

}

void FormGPS::FileSaveNMEA()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "NMEA_log.txt");

    QFile nmeafile(filename);
    if (!nmeafile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&nmeafile);
    writer.setLocale(QLocale::C);

    writer << pn.logNMEASentence;

    pn.logNMEASentence.clear();

    nmeafile.close();
}

void FormGPS::FileSaveElevation()
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "Elevation.txt");

    QFile elevfile(filename);
    if (!elevfile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&elevfile);
    writer.setLocale(QLocale::C);

    writer << sbFix;

    sbFix.clear();

    elevfile.close();
}

void FormGPS::FileSaveSingleFlagKML2(int flagNumber)
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, QString("Flag%1.kml").arg(flagNumber));


    QFile kmlFile(filename);
    if (!kmlFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&kmlFile);
    writer.setLocale(QLocale::C);

    writer << "<?xml version=""1.0"" encoding=""UTF-8""?" << Qt::endl;
    writer << "<kml xmlns=""http://www.opengis.net/kml/2.2""> " << Qt::endl;

    //int count2 = flagPts.count();
    double lat, lon;

    pn.ConvertLocalToWGS84(flagPts[flagNumber - 1].northing, flagPts[flagNumber - 1].easting, lat, lon);

    writer << "<Document>" << Qt::endl;

    writer << "<Placemark>"  << Qt::endl;;
    writer << "<Style><IconStyle>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 0)  //red - xbgr
        writer << "<color>ff4400ff</color>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 1)  //grn - xbgr
        writer << "<color>ff44ff00</color>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 2)  //yel - xbgr
        writer << "<color>ff44ffff</color>" << Qt::endl;
    writer << "</IconStyle></Style>" << Qt::endl;
    writer << "<name>" << flagNumber << "</name>" << Qt::endl;
    writer << "<Point><coordinates>" << lon << "," << lat << ",0"
           << "</coordinates></Point>" << Qt::endl;
    writer << "</Placemark>" << Qt::endl;
    writer << "</Document>" << Qt::endl;
    writer << "</kml>" << Qt::endl;
}

void FormGPS::FileSaveSingleFlagKML(int flagNumber)
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, QString("Flag%1.kml").arg(flagNumber));


    QFile kmlFile(filename);
    if (!kmlFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&kmlFile);
    writer.setLocale(QLocale::C);

    writer << "<?xml version=""1.0"" encoding=""UTF-8""?" << Qt::endl;
    writer << "<kml xmlns=""http://www.opengis.net/kml/2.2""> " << Qt::endl;

    //int count2 = flagPts.count();

    writer << "<Document>" << Qt::endl;

    writer << "<Placemark>"  << Qt::endl;;
    writer << "<Style><IconStyle>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 0)  //red - xbgr
        writer << "<color>ff4400ff</color>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 1)  //grn - xbgr
        writer << "<color>ff44ff00</color>" << Qt::endl;
    if (flagPts[flagNumber - 1].color == 2)  //yel - xbgr
        writer << "<color>ff44ffff</color>" << Qt::endl;
    writer << "</IconStyle></Style>" << Qt::endl;
    writer << "<name>" << flagNumber << "</name>" << Qt::endl;
    writer << "<Point><coordinates>"
           << flagPts[flagNumber-1].longitude << ","
           << flagPts[flagNumber-1].latitude << ",0"
           << "</coordinates></Point>" << Qt::endl;
    writer << "</Placemark>" << Qt::endl;
    writer << "</Document>" << Qt::endl;
    writer << "</kml>" << Qt::endl;
}

void FormGPS::FileMakeKMLFromCurrentPosition(double lat, double lon)
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, "CurrentPosition.kml");


    QFile kmlFile(filename);
    if (!kmlFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&kmlFile);
    writer.setLocale(QLocale::C);

    writer << "<?xml version=""1.0"" encoding=""UTF-8""?>     " << Qt::endl;
    writer << "<kml xmlns=""http://www.opengis.net/kml/2.2""> " << Qt::endl;

    writer << "<Document>" << Qt::endl;
    writer << "<Placemark>" << Qt::endl;
    writer << "<Style> <IconStyle>" << Qt::endl;
    writer << "<color>ff4400ff</color>" << Qt::endl;
    writer << "</IconStyle></Style>" << Qt::endl;
    writer << "<name>Your Current Position</name>" << Qt::endl;
    writer << "<Point><coordinates> "
           << lon << "," << lat << ",0"
           << "</coordinates></Point>" << Qt::endl;
    writer << "</Placemark>" << Qt::endl;
    writer << "</Document>" << Qt::endl;
    writer << "</kml>" << Qt::endl;


}

void FormGPS::ExportFieldAs_KML()
{
    //TODO:  use XML library
}

QString FormGPS::GetBoundaryPointsLatLon(int bndNum)
{
    QString sb;
    QTextStream sb_writer(&sb);
    double lat = 0;
    double lon = 0;

    for (int i = 0; i < bnd.bndList[bndNum].fenceLine.count(); i++)
    {
        pn.ConvertLocalToWGS84(bnd.bndList[bndNum].fenceLine[i].northing, bnd.bndList[bndNum].fenceLine[i].easting, lat, lon);
        sb_writer << qSetRealNumberPrecision(7)
                  << lon << ','
                  << lat << ",0 "
                  << Qt::endl; // TODO: should this be here?
    }

    return sb;
}

void FormGPS::FileUpdateAllFieldsKML()
{
    //Update or add the current field to the Field.kml file
    //TODO: use XML library
}

