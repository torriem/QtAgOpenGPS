#include "formgps.h"
#include <QDir>
#include "aogsettings.h"
#include "cmodulecomm.h"

void FormGPS::fileSaveCurveLines()
{
    curve.moveDistance = 0;

    QString directoryName = QStandardPaths::DocumentsLocation + "/" +
            QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/CurveLines.txt";

    int cnt = curve.curveArr.count();
    curve.numCurveLines = cnt;

    QFile curveFile(filename);
    if (!curveFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&curveFile);

    writer << "$CurveLines" << endl;

    if (cnt > 0)
    {
        for (int i = 0; i < cnt; i++)
        {
            //write out the Name
            writer << curve.curveArr[i].Name << endl;

            //write out the aveheading
            writer << curve.curveArr[i].aveHeading << endl;

            //write out the points of ref line
            int cnt2 = curve.curveArr[i].curvePts.count();

            writer << cnt2 << endl;
            if (curve.curveArr[i].curvePts.count() > 0)
            {
                for (int j = 0; j < cnt2; j++)
                    writer << qSetFieldWidth(3) << curve.curveArr[i].curvePts[j].easting << ","
                           << qSetFieldWidth(3) << curve.curveArr[i].curvePts[j].northing << ","
                           << qSetFieldWidth(5) << curve.curveArr[i].curvePts[j].heading << endl;
            }
        }
    }

    if (curve.numCurveLines == 0) curve.numCurveLineSelected = 0;
    if (curve.numCurveLineSelected > curve.numCurveLines) curve.numCurveLineSelected = curve.numCurveLines;

    curveFile.close();
}

void FormGPS::fileLoadCurveLines()
{
    curve.moveDistance = 0;
    curve.curveArr.clear();
    curve.numCurveLines = 0;

    //current field directory should already exist
    QString directoryName = QStandardPaths::DocumentsLocation + "/" +
            QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/CurveLines.txt";

    QFile curveFile(filename);
    if (!curveFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&curveFile);

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

void FormGPS::fileSaveABLines()
{
    ABLine.moveDistance = 0;

    QString directoryName = QStandardPaths::DocumentsLocation + "/" +
            QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/ABLines.txt";

    QFile lineFile(filename);
    if (!lineFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&lineFile);

    int cnt = ABLine.lineArr.count();

    if (cnt > 0)
    {
        foreach (CABLines item, ABLine.lineArr)
        {
            //make it culture invariant
            writer << item.Name << ","
                   << qSetFieldWidth(8) << glm::toDegrees(item.heading) << ","
                   << qSetFieldWidth(3) << item.origin.easting << ","
                   << qSetFieldWidth(3) << item.origin.northing << endl;
        }
    }

    if (ABLine.numABLines == 0) ABLine.numABLineSelected = 0;
    if (ABLine.numABLineSelected > ABLine.numABLines) ABLine.numABLineSelected = ABLine.numABLines;

    lineFile.close();
}

void FormGPS::fileLoadABLines()
{
    ABLine.moveDistance = 0;

    //current field directory should already exist
    QString directoryName = QStandardPaths::DocumentsLocation + "/" +
            QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/ABLines.txt";

    QFile linesFile(filename);
    if (!linesFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&linesFile);

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

        ABLine.lineArr[i].ref1.easting = ABLine.lineArr[i].origin.easting - (sin(ABLine.lineArr[i].heading) * 1000.0);
        ABLine.lineArr[i].ref1.northing = ABLine.lineArr[i].origin.northing - (cos(ABLine.lineArr[i].heading) *1000.0);

        ABLine.lineArr[i].ref2.easting = ABLine.lineArr[i].origin.easting + (sin(ABLine.lineArr[i].heading) * 1000.0);
        ABLine.lineArr[i].ref2.northing = ABLine.lineArr[i].origin.northing + (cos(ABLine.lineArr[i].heading) * 1000.0);
        ABLine.numABLines++;
    }

    if (ABLine.numABLines == 0) ABLine.numABLineSelected = 0;
    if (ABLine.numABLineSelected > ABLine.numABLines) ABLine.numABLineSelected = ABLine.numABLines;

    linesFile.close();
}

void FormGPS::fileSaveVehicle(QString filename)
{
    USE_SETTINGS;

    vehicleFileName = QFileInfo(filename).baseName() + " - ";
    SETTINGS_SET_VEHICLE_NAME(vehicleFileName);

    QFile saveFile(filename);
    if( ! saveFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open " << filename << " for reading.";
        //TODO pop up error message
        return;
    }

    QTextStream writer(&saveFile);

    writer << "Version," << QCoreApplication::applicationVersion() << endl;
    writer << "AntennaHeight," << SETTINGS_VEHICLE_ANTENNAHEIGHT << endl;
    writer << "AntennaPivot," << SETTINGS_VEHICLE_ANTENNAPIVOT << endl;
    writer << "AntennaOffset," << SETTINGS_VEHICLE_ANTENNAOFFSET << endl;
    writer << "IsPivotBehindAntenna," << (SETTINGS_VEHICLE_ISPIVOTBEHINDANTENNA ? "True" : "False") << endl;
    writer << "IsSteerAxleAhead," << (SETTINGS_VEHICLE_ISSTEERAXLEAHEAD ? "True" : "False") << endl;

    writer << "Wheelbase," << SETTINGS_VEHICLE_WHEELBASE << endl;
    writer << "MinTurningRadius," << SETTINGS_VEHICLE_MINTURNINGRADIUS << endl;
    writer << "MinFixStep," << SETTINGS_VEHICLE_MINFIXSTEP << endl;
    writer << "LowSpeedCutoff," << SETTINGS_VEHICLE_SLOWSPEEDCUTOFF << endl;
    writer << "VehicleType," << SETTINGS_VEHICLE_TYPE << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "GeoFenceDistance," << SETTINGS_VEHICLE_GEOFENCEDIST << endl;
    writer << "UTurnSkipWidth," << SETTINGS_VEHICLE_YOUSKIPWIDTH << endl;
    writer << "YouTurnDistance," << SETTINGS_VEHICLE_YOUTURNDISTANCE << endl;
    writer << "youTriggerDistance," << SETTINGS_VEHICLE_YOUTURNTRIGGERDISTANCE << endl;
    writer << "YouTurnUseDubins," << SETTINGS_VEHICLE_ISUSINGDUBINSTURN << endl;
    writer << "IsMachineControlToAS," << SETTINGS_VEHICLE_ISMACHINECONTROLTOAUTOSTEER << endl;

    //AutoSteer
    writer << "pidP," << SETTINGS_AUTOSTEER_KP << endl;
    writer << "pidI," << SETTINGS_AUTOSTEER_KI << endl;
    writer << "pidD," << SETTINGS_AUTOSTEER_KD << endl;
    writer << "pidO," << SETTINGS_AUTOSTEER_KO << endl;
    writer << "SteerAngleOffset," << SETTINGS_AUTOSTEER_STEERINGANGLEOFFSET << endl;
    writer << "minPWM," << SETTINGS_AUTOSTEER_MINSTEERPWM << endl;
    writer << "MaxIntegral," << SETTINGS_AUTOSTEER_MAXINTEGRAL << endl;
    writer << "CountsPerDegree," << SETTINGS_AUTOSTEER_COUNTSPERDEGREE << endl;
    writer << "MaxSteerAngle," << SETTINGS_VEHICLE_MAXSTEERANGLE << endl;
    writer << "MaxAngularVelocity," << SETTINGS_VEHICLE_MAXANGULARVELOCITY << endl;
    writer << "IsJRK," << SETTINGS_AUTOSTEER_ISJRK << endl;
    writer << "SnapDistance," << SETTINGS_AUTOSTEER_SNAPDISTANCE << endl;

    writer << "isStanleyUsed," << SETTINGS_VEHICLE_ISSTANLEYUSED << endl;
    writer << "StanleyGain," << SETTINGS_VEHICLE_STANLEYGAIN << endl;
    writer << "StanleyHeadingError," << SETTINGS_VEHICLE_STANLEYHEADINGERRORGAIN << endl;

    writer << "GoalPointLookAhead," << SETTINGS_VEHICLE_GOALPOINTLOOKAHEAD << endl;

    writer << "GoalPointLookAheadUTurnMult," << SETTINGS_VEHICLE_LOOKAHEADUTURNMULT << endl;

    writer << "GoalPointLookAheadMinumum," << SETTINGS_VEHICLE_LOOKAHEADMINIMUM << endl;

    writer << "GoalPointLookAheadDistanceFromLine," << SETTINGS_VEHICLE_DISTANCEMULTIPLIER << endl;

    writer << "HydLiftLookAhead," << SETTINGS_VEHICLE_HYDLIFTLOOKAHEAD << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //IMU
    writer << "HeadingFromSource," << SETTINGS_GPS_HEADINGFROMWHICHSOURCE << endl;
    writer << "GPSWhichSentence," << SETTINGS_GPS_FIXFROMWHICH << endl;

    writer << "HeadingFromBrick," << SETTINGS_GPS_ISHEADINGCORRECTIONFROMBRICK << endl;
    writer << "RollFromAutoSteer," << SETTINGS_GPS_ISROLLFROMAUTOSTEER << endl;
    writer << "HeadingFromAutoSteer," << SETTINGS_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER << endl;
    writer << "IMUPitchZero," << SETTINGS_GPS_IMUPITCHZEROX16 << endl;
    writer << "IMURollZero," << SETTINGS_GPS_IMUROLLZEROX16 << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //Arduino steer Config
    writer << "ArduinoInclinometer," << SETTINGS_ARDSTEER_INCLINOMETER << endl;
    writer << "ArduinoMaxPulseCounts," << SETTINGS_ARDSTEER_MAXPULSECOUNTS << endl;
    writer << "ArduinoMaxSpeed," << SETTINGS_ARDSTEER_MAXSPEED << endl;
    writer << "ArduinoMinSpeed," << SETTINGS_ARDSTEER_MINSPEED << endl;
    writer << "ArduinoSetting0," << SETTINGS_ARDSTEER_SETTING0 << endl;
    writer << "ArduinoSetting1," << SETTINGS_ARDSTEER_SETTING1 << endl;
    writer << "ArduinoAckermanFix," << SETTINGS_ARDSTEER_ACKERMANFIX << endl;

    //Arduino Machine Config
    writer << "ArdMachineRaiseTime," << SETTINGS_ARDMAC_HYDRAISETIME << endl;
    writer << "ArdMachineLowerTime," << SETTINGS_ARDMAC_HYDLOWERTIME << endl;
    writer << "ArdMachineEnableHydraulics," << SETTINGS_ARDMAC_ISHYDENABLED << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //uturn sequences
    writer << "SequenceFunctionEnter;" << SETTINGS_VEHICLE_SEQFUNCTIONENTER << endl;
    writer << "SequenceFunctionExit;" << SETTINGS_VEHICLE_SEQFUNCTIONEXIT << endl;
    writer << "SequenceActionEnter;" << SETTINGS_VEHICLE_SEQACTIONENTER << endl;
    writer << "SequenceActionExit;" << SETTINGS_VEHICLE_SEQACTIONEXIT << endl;
    writer << "SequenceDistanceEnter;" << SETTINGS_VEHICLE_SEQDISTANCEENTER << endl;
    writer << "SequenceDistanceExit;" << SETTINGS_VEHICLE_SEQDISTANCEEXIT << endl;

    writer << "FunctionList;" << SETTINGS_VEHICLE_SEQFUNCTIONLIST << endl;
    writer << "ActionList;" << SETTINGS_VEHICLE_SEQACTIONLIST << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //little show to say saved and where
    //TODO var form = new FormTimedMessage(3000, gStr.gsSavedInFolder, vehiclesDirectory);

    saveFile.close();
}

bool FormGPS::fileOpenVehicle(QString filename)
{
    USE_SETTINGS;

    QFile openFile(filename);
    if(! openFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open " << filename << " for reading.";
        //TODO: popup error message
        return false;
    }

    QString line;

    line = QFileInfo(filename).baseName() + " - ";
    SETTINGS_SET_VEHICLE_NAME(filename);

    QTextStream reader(&openFile);

    QStringList words;

    line = reader.readLine(); words = line.split(',');

    //if (words[0] != "Version")

    //{
    //    var form = new FormTimedMessage(2000, gStr.gsVehicleFileIsWrongVersion, gStr.gsMustBeVersion + Application.ProductVersion.ToString(CultureInfo.InvariantCulture) + " or higher");
    //    form.Show();
    //    return false;
    //}

    QString vers = words[1];
    vers = vers.replace('.','0');
    int fileVersion = vers.toInt();

    QString assemblyVersion = QCoreApplication::applicationVersion();
    assemblyVersion = assemblyVersion.replace('.', '0');
    int appVersion = assemblyVersion.toInt();

    appVersion /= 100;
    fileVersion /= 100;

    if (fileVersion < appVersion)
    {
        qDebug() << "Saved vehicle file " << filename << " is in an older format and cannot be read.";
        //TODO error message
        return false;
    }
    else
    {
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_ANTENNAHEIGHT(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_ANTENNAPIVOT(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_ANTENNAOFFSET(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        if (words[1].toLower().trimmed() == "true")
            SETTINGS_SET_VEHICLE_ISPIVOTBEHINDANTENNA(true);
        else
            SETTINGS_SET_VEHICLE_ISPIVOTBEHINDANTENNA(false);

        line = reader.readLine(); words = line.split(',');

        if (words[1].toLower().trimmed() == "true")
            SETTINGS_SET_VEHICLE_ISSTEERAXLEAHEAD(true);
        else
            SETTINGS_SET_VEHICLE_ISSTEERAXLEAHEAD(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_WHEELBASE(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_MINTURNINGRADIUS(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_MINFIXSTEP(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_SLOWSPEEDCUTOFF(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_TYPE(words[1].toInt());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_GEOFENCEDIST(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_YOUSKIPWIDTH(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_YOUTURNDISTANCE(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_YOUTURNTRIGGERDISTANCE(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');

        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_VEHICLE_ISUSINGDUBINSTURN(true);
        else
            SETTINGS_SET_VEHICLE_ISUSINGDUBINSTURN(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_VEHICLE_ISMACHINECONTROLTOAUTOSTEER(true);
        else
            SETTINGS_SET_VEHICLE_ISMACHINECONTROLTOAUTOSTEER(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_KP(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_KI(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_KD(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_KO(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_STEERINGANGLEOFFSET(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_MINSTEERPWM(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_MAXINTEGRAL(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_COUNTSPERDEGREE(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_MAXSTEERANGLE(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_MAXANGULARVELOCITY(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');

        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_AUTOSTEER_ISJRK(true);
        else
            SETTINGS_SET_AUTOSTEER_ISJRK(true);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_AUTOSTEER_SNAPDISTANCE(words[1].toInt());


        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_VEHICLE_ISSTANLEYUSED(true);
        else
            SETTINGS_SET_VEHICLE_ISSTANLEYUSED(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_STANLEYGAIN(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_STANLEYHEADINGERRORGAIN(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_GOALPOINTLOOKAHEAD(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_LOOKAHEADUTURNMULT(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_LOOKAHEADMINIMUM(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_DISTANCEMULTIPLIER(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_VEHICLE_HYDLIFTLOOKAHEAD(words[1].toDouble());

        //line = reader.readLine(); words = line.split(',');
        //if (words[0] == "Empty") SETTINGS_SET_VEHICLE_lookAheadDistanceFromLine = 1.2;
        //else SETTINGS_SET_VEHICLE_lookAheadDistanceFromLine = double.Parse(words[1], CultureInfo.InvariantCulture);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_HEADINGFROMWHICHSOURCE(words[1]);
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_FIXFROMWHICH(words[1]);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMBRICK(true);
        else
            SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMBRICK(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(true);
        else
            SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(true);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER(true);
        else
            SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_IMUPITCHZEROX16(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_IMUROLLZEROX16(words[1].toInt());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();


        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_INCLINOMETER(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_MAXPULSECOUNTS(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_MAXSPEED(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_MINSPEED(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_SETTING0(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_SETTING1(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDSTEER_ACKERMANFIX(words[1].toInt());

        //Arduino Machine Config
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDMAC_HYDRAISETIME(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDMAC_HYDLOWERTIME(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_ARDMAC_ISHYDENABLED(words[1].toInt());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQFUNCTIONENTER(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQFUNCTIONEXIT(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQACTIONENTER(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQACTIONEXIT(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQDISTANCEENTER(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQDISTANCEEXIT(words[1]);

        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQFUNCTIONLIST(words[1]);
        line = reader.readLine(); words = line.split(';');
        SETTINGS_SET_VEHICLE_SEQACTIONLIST(words[1]);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        mc.autoSteerSettings[mc.ssKp] = SETTINGS_AUTOSTEER_KP;
        mc.autoSteerSettings[mc.ssKi] = SETTINGS_AUTOSTEER_KI;
        mc.autoSteerSettings[mc.ssKd] = SETTINGS_AUTOSTEER_KD;
        mc.autoSteerSettings[mc.ssKo] = SETTINGS_AUTOSTEER_KO;
        mc.autoSteerSettings[mc.ssSteerOffset] = SETTINGS_AUTOSTEER_STEERINGANGLEOFFSET;
        mc.autoSteerSettings[mc.ssMinPWM] = SETTINGS_AUTOSTEER_MINSTEERPWM;
        mc.autoSteerSettings[mc.ssMaxIntegral] = SETTINGS_AUTOSTEER_MAXINTEGRAL;
        mc.autoSteerSettings[mc.ssCountsPerDegree] = SETTINGS_AUTOSTEER_COUNTSPERDEGREE;

        headingFromSource = SETTINGS_GPS_HEADINGFROMWHICHSOURCE;
        pn.fixFrom = SETTINGS_GPS_FIXFROMWHICH;

        mc.ardSteerConfig[mc.arHeaderHi] = 127; //PGN - 32750
        mc.ardSteerConfig[mc.arHeaderLo] = 238;
        mc.ardSteerConfig[mc.arSet0] = SETTINGS_ARDSTEER_SETTING0;
        mc.ardSteerConfig[mc.arSet1] = SETTINGS_ARDSTEER_SETTING1;
        mc.ardSteerConfig[mc.arMaxSpd] = SETTINGS_ARDSTEER_MAXSPEED;
        mc.ardSteerConfig[mc.arMinSpd] = SETTINGS_ARDSTEER_MINSPEED;
        mc.ardSteerConfig[mc.arAckermanFix] = SETTINGS_ARDSTEER_ACKERMANFIX;

        uchar inc = (uchar)SETTINGS_ARDSTEER_INCLINOMETER << 6;
        mc.ardSteerConfig[mc.arIncMaxPulse] = inc + (uchar)SETTINGS_ARDSTEER_MAXPULSECOUNTS;

        mc.ardSteerConfig[mc.arAckermanFix] = 0;
        mc.ardSteerConfig[mc.ar8] = 0;
        mc.ardSteerConfig[mc.ar9] = 0;

        mc.ardMachineConfig[mc.amHeaderHi] = 127; //PGN - 32760
        mc.ardMachineConfig[mc.amHeaderLo] = 248;
        mc.ardMachineConfig[mc.amRaiseTime] = SETTINGS_ARDMAC_HYDRAISETIME;
        mc.ardMachineConfig[mc.amLowerTime] = SETTINGS_ARDMAC_HYDLOWERTIME;
        mc.ardMachineConfig[mc.amEnableHyd] = SETTINGS_ARDMAC_ISHYDENABLED;
        mc.ardMachineConfig[mc.am5] = 0;
        mc.ardMachineConfig[mc.am6] = 0;
        mc.ardMachineConfig[mc.am7] = 0;
        mc.ardMachineConfig[mc.am8] = 0;
        mc.ardMachineConfig[mc.am9] = 0;

        words = SETTINGS_VEHICLE_SEQFUNCTIONENTER.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++) seq.seqEnter[i].function = words[i].toInt();

        words = SETTINGS_VEHICLE_SEQACTIONENTER.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++) seq.seqEnter[i].action = words[i].toInt();

        words = SETTINGS_VEHICLE_SEQDISTANCEENTER.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++)
            seq.seqEnter[i].distance = words[i].toDouble();

        words = SETTINGS_VEHICLE_SEQFUNCTIONEXIT.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++) seq.seqExit[i].function = words[i].toInt();

        words = SETTINGS_VEHICLE_SEQACTIONEXIT.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++) seq.seqExit[i].action = words[i].toInt();

        words = SETTINGS_VEHICLE_SEQDISTANCEEXIT.split(',');
        for (int i = 0; i < MAXFUNCTIONS; i++)
            seq.seqExit[i].distance = words[i].toDouble();
    }
    return true;

}
