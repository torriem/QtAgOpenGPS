#include "formgps.h"
#include <QDir>
#include "aogsettings.h"
#include "cmodulecomm.h"

void FormGPS::fileSaveCurveLines()
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
    qDebug() << directoryName;

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
                    writer << qSetRealNumberPrecision(3) << curve.curveArr[i].curvePts[j].easting << ","
                           << qSetRealNumberPrecision(3) << curve.curveArr[i].curvePts[j].northing << ","
                           << qSetRealNumberPrecision(5) << curve.curveArr[i].curvePts[j].heading << endl;
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
                   << qSetRealNumberPrecision(8) << glm::toDegrees(item.heading) << ","
                   << qSetRealNumberPrecision(3) << item.origin.easting << ","
                   << qSetRealNumberPrecision(3) << item.origin.northing << endl;
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
    if( ! saveFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open " << filename << " for writing.";
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
    writer << "LowSpeedCutoff," << SETTINGS_TOOL_SLOWSPEEDCUTOFF << endl;
    writer << "VehicleType," << SETTINGS_VEHICLE_TYPE << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "GeoFenceDistance," << SETTINGS_VEHICLE_GEOFENCEDIST << endl;
    writer << "UTurnSkipWidth," << SETTINGS_VEHICLE_YOUSKIPWIDTH << endl;
    writer << "YouTurnDistance," << SETTINGS_VEHICLE_YOUTURNDISTANCE << endl;
    writer << "youTriggerDistance," << SETTINGS_VEHICLE_YOUTURNTRIGGERDISTANCE << endl;
    writer << "YouTurnUseDubins," << (SETTINGS_VEHICLE_ISUSINGDUBINSTURN ? "True" : "False") << endl;
    writer << "IsMachineControlToAS," << (SETTINGS_VEHICLE_ISMACHINECONTROLTOAUTOSTEER ? "True" : "False") << endl;

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
    writer << "IsJRK," << (SETTINGS_AUTOSTEER_ISJRK ? "True" : "False") << endl;
    writer << "SnapDistance," << SETTINGS_AUTOSTEER_SNAPDISTANCE << endl;

    writer << "isStanleyUsed," << (SETTINGS_VEHICLE_ISSTANLEYUSED ? "True" : "False") << endl;
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
    SETTINGS_SET_VEHICLE_NAME(line);

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
            SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(false);

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

void FormGPS::fileSaveTool(QString filename)
{
    USE_SETTINGS;
    toolFileName = QFileInfo(filename).baseName() + " - ";

    SETTINGS_SET_TOOL_NAME(toolFileName);
    
    QFile saveFile(filename);
    if( ! saveFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open " << filename << " for writing.";
        //TODO pop up error message
        return;
    }

    QTextStream writer(&saveFile);
 
    writer << "Version," << QCoreApplication::applicationVersion() << endl;

    writer << "Overlap," << SETTINGS_TOOL_OVERLAP << endl;
    writer << "ToolOffset," << SETTINGS_TOOL_OFFSET << endl;

    writer << "LookAheadOff," << SETTINGS_TOOL_LOOKAHEADOFF << endl;
    writer << "LookAheadOn," << SETTINGS_TOOL_LOOKAHEADON << endl;
    writer << "TurnOffDelay," << SETTINGS_TOOL_OFFDELAY << endl;
    writer << "ToolMinUnappliedPixels," << SETTINGS_TOOL_MINAPPLIED << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

	writer << "ToolTrailingHitchLength," << SETTINGS_TOOL_TRAILINGHITCHLENGTH << endl;
	writer << "TankTrailingHitchLength," << SETTINGS_TOOL_TANKTRAILINGHITCHLENGTH << endl;
    writer << "HitchLength," << SETTINGS_TOOL_HITCHLENGTH << endl;

    writer << "IsToolBehindPivot," << (SETTINGS_TOOL_ISBEHINDPIVOT ? "True": "False") << endl;
    writer << "IsToolTrailing," << (SETTINGS_TOOL_ISTRAILING ? "True": "False")  << endl;
    writer << "IsToolTBT," << (SETTINGS_TOOL_ISTBT ? "True": "False")  << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "Spinner1," << SETTINGS_TOOL_SECTIONPOSITION1 << endl;
    writer << "Spinner2," << SETTINGS_TOOL_SECTIONPOSITION2 << endl;
    writer << "Spinner3," << SETTINGS_TOOL_SECTIONPOSITION3 << endl;
    writer << "Spinner4," << SETTINGS_TOOL_SECTIONPOSITION4 << endl;
    writer << "Spinner5," << SETTINGS_TOOL_SECTIONPOSITION5 << endl;
    writer << "Spinner6," << SETTINGS_TOOL_SECTIONPOSITION6 << endl;
    writer << "Spinner7," << SETTINGS_TOOL_SECTIONPOSITION7 << endl;
    writer << "Spinner8," << SETTINGS_TOOL_SECTIONPOSITION8 << endl;
    writer << "Spinner9," << SETTINGS_TOOL_SECTIONPOSITION9 << endl;
    writer << "Spinner10," << SETTINGS_TOOL_SECTIONPOSITION10 << endl;
    writer << "Spinner11," << SETTINGS_TOOL_SECTIONPOSITION11 << endl;
    writer << "Spinner12," << SETTINGS_TOOL_SECTIONPOSITION12 << endl;
    writer << "Spinner13," << SETTINGS_TOOL_SECTIONPOSITION13 << endl;
    writer << "Spinner14," << SETTINGS_TOOL_SECTIONPOSITION14 << endl;
    writer << "Spinner15," << SETTINGS_TOOL_SECTIONPOSITION15 << endl;
    writer << "Spinner16," << SETTINGS_TOOL_SECTIONPOSITION16 << endl;
    writer << "Spinner17," << SETTINGS_TOOL_SECTIONPOSITION17 << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;


	writer << "Sections," << SETTINGS_TOOL_NUMSECTIONS << endl;
	writer << "ToolWidth," << SETTINGS_TOOL_WIDTH << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "WorkSwitch," << ( SETTINGS_TOOL_ISWORKSWITCHENABLED ? "True" : "False" ) << endl;
    writer << "ActiveLow," << ( SETTINGS_TOOL_ISWORKSWITCHACTIVELOW ? "True" : "False" ) << endl;
    writer << "SwitchManual," << ( SETTINGS_TOOL_ISWORKSWITCHMANUAL ? "True" : "False" ) << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //little show to say saved and where
    //TODO: show message var form = new FormTimedMessage(3000, gStr.gsSavedInFolder, toolsDirectory);

}

bool FormGPS::fileOpenTool(QString filename)
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
    SETTINGS_SET_TOOL_NAME(line);
    toolFileName = line; //remove this and use setting directly in other code

    QTextStream reader(&openFile);

    QStringList words;

    line = reader.readLine(); words = line.split(',');

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
        qDebug() << "Saved tool file " << filename << " is in an older format and cannot be read.";
        //TODO error message
        return false;
    }
    else
    {
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_OVERLAP(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_OFFSET(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_LOOKAHEADOFF(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_LOOKAHEADON(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_OFFDELAY(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_MINAPPLIED(words[1].toDouble());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_TRAILINGHITCHLENGTH(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_TANKTRAILINGHITCHLENGTH(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_HITCHLENGTH(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISBEHINDPIVOT(true);
        else
            SETTINGS_SET_TOOL_ISBEHINDPIVOT(false);
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISTRAILING(true);
        else
            SETTINGS_SET_TOOL_ISTRAILING(false);
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISTBT(true);
        else
            SETTINGS_SET_TOOL_ISTBT(false);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION1 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION2 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION3 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION4 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION5 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION6 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION7 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION8 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION9 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION10 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION11 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION12 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION13 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION14 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION15 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION16 (words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_SECTIONPOSITION17 (words[1].toDouble());


        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_NUMSECTIONS(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_TOOL_WIDTH(words[1].toDouble());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISWORKSWITCHENABLED(true);
        else
            SETTINGS_SET_TOOL_ISWORKSWITCHENABLED(false);
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISWORKSWITCHACTIVELOW(true);
        else
            SETTINGS_SET_TOOL_ISWORKSWITCHACTIVELOW(false);
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_TOOL_ISWORKSWITCHMANUAL(true);
        else
            SETTINGS_SET_TOOL_ISWORKSWITCHMANUAL(false);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        SYNC_SETTINGS;

        //Set width of section and positions for each section
        tool.sectionSetPositions();

        //Calculate total width and each section width
        tool.sectionCalcWidths();

        //enable disable manual buttons
        lineUpManualBtns();

        return true;
    }
}

void FormGPS::fileSaveEnvironment(QString filename)
{
    USE_SETTINGS;
    QString envFileName = QFileInfo(filename).baseName() + " - ";

    SETTINGS_SET_DISPLAY_NAME(envFileName);

    QFile saveFile(filename);
    if( ! saveFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open " << filename << " for writing.";
        //TODO pop up error message
        return;
    }

    QTextStream writer(&saveFile);

    writer << "Version," << QCoreApplication::applicationVersion() << endl;

    writer << "Culture,en_US" << endl; //for now just use system locale always, ignore this
    writer << "CamPitch," << SETTINGS_DISPLAY_CAMPITCH << endl;
    writer << "IsBatmanOn," << SETTINGS_DISPLAY_ISBATMANON << endl;
    writer << "LightBarCMPerPixel," << SETTINGS_DISPLAY_LIGHTBARCMPP << endl;
    writer << "LineWidth," << SETTINGS_DISPLAY_LINEWIDTH << endl;

    writer << "IsCompassOn," << SETTINGS_DISPLAY_COMPASS << endl;
    writer << "IsGridOn," << SETTINGS_DISPLAY_SHOWGRID << endl;

    writer << "IsLightBarOn," << SETTINGS_DISPLAY_LIGHTBARON << endl;
    writer << "IsLogNMEA," << SETTINGS_GPS_LOGNMEA << endl;
    writer << "IsMetric," << SETTINGS_DISPLAY_ISMETRIC << endl;
    writer << "IsOGLZoom," << SETTINGS_DISPLAY_OGLZOOM << endl;

    writer << "IsPurePursuitLineOn," << SETTINGS_DISPLAY_ISPUREON << endl;
    writer << "IsGuideLinesOn," << SETTINGS_DISPLAY_SIDEGUIDELINES << endl;
    writer << "IsSimulatorOn," << SETTINGS_SIM_ON << endl;
    writer << "IsSkyOn," << SETTINGS_DISPLAY_SKYON << endl;
    writer << "IsSpeedoOn," << SETTINGS_DISPLAY_SPEEDO << endl;
    writer << "IsUTurnAlwaysOn," << SETTINGS_DISPLAY_UTURNALWAYSON << endl;
    writer << "IsAutoDayNightModeOn," << SETTINGS_DISPLAY_ISAUTODAYNIGHT << endl;
    writer << "StartFullScreen," << SETTINGS_DISPLAY_FULLSCREEN << endl;
    writer << "IsRTKOn," << SETTINGS_GPS_EXPECTRTK << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    writer << "IsNtripCasterIP," << SETTINGS_GPS_NTRIPCASTERIP << endl;
    writer << "IsNtripCasterPort," << SETTINGS_GPS_NTRIPCASTERPORT << endl;
    writer << "IsNtripCasterURL," << SETTINGS_GPS_NTRIPCASTERURL << endl;
    writer << "IsNtripGGAManual," << SETTINGS_GPS_NTRIPGGAMANUAL << endl;
    writer << "IsNtripOn," << SETTINGS_GPS_NTRIPON << endl;
    writer << "IsNtripTCP," << SETTINGS_GPS_NTRIPTCP << endl;
    writer << "IsNtripManualLat," << SETTINGS_GPS_NTRIPMANUALLAT << endl;
    writer << "IsNtripManualLon," << SETTINGS_GPS_NTRIPMANUALLON << endl;
    writer << "IsNtripMount," << SETTINGS_GPS_NTRIPMOUNT << endl;
    writer << "IsNtripGGAInterval," << SETTINGS_GPS_NTRIPGGAINTERVAL << endl;
    writer << "IsNtripSendToUDPPort," << SETTINGS_GPS_NTRIPSENDTOUDPPORT << endl;
    writer << "IsNtripUserName," << SETTINGS_GPS_NTRIPUSERNAME << endl;
    writer << "IsNtripUserPassword," << SETTINGS_GPS_NTRIPPASSWORD << endl;

    writer << "IsUDPOn,True" << endl; //TODO. Where's the port number?

    writer << "GPSSimLatitude," << SETTINGS_SIM_LATITUDE << endl;
    writer << "GPSSimLongitude," << SETTINGS_SIM_LONGITUDE << endl;


    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;


    writer << "FieldColorDay," << colorSettingStringToInt( SETTINGS_DISPLAY_FIELDCOLORDAY) << endl;
    writer << "SectionColorDay," << colorSettingStringToInt( SETTINGS_DISPLAY_SECTIONSCOLORDAY) << endl;
    writer << "FieldColorNight," << colorSettingStringToInt( SETTINGS_DISPLAY_FIELDCOLORNIGHT) << endl;
    writer << "SectionColorNight," << colorSettingStringToInt( SETTINGS_DISPLAY_SECTIONSCOLORNIGHT) << endl;
    writer << "DayColor," << colorSettingStringToInt( SETTINGS_DISPLAY_COLORDAY) << endl;
    writer << "NightColor," << colorSettingStringToInt( SETTINGS_DISPLAY_COLORNIGHT) << endl;
    writer << "IsSimple," << SETTINGS_DISPLAY_ISSIMPLE << endl;
    writer << "IsDayMode," << SETTINGS_DISPLAY_ISDAYMODE << endl;
    //TODO support custom color palette
    writer << "CustomColors,10130518,7843687,8605795,6170168,3758726,3552822,8826561,15156186,4351583,162626,5317709,7629648,7696185,5789221,14993507,11730944" << endl;

    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;
    writer << "Empty," << "10" << endl;

    //TODO: message indicating success
}

bool FormGPS::fileOpenEnvironment(QString filename)
{
    USE_SETTINGS;

    QFile openFile(filename);
    if(! openFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open environment file " << filename << " for reading.";
        //TODO: popup error message
        return false;
    }
    QTextStream reader(&openFile);

    QString line;


    QStringList words;
    line = reader.readLine(); words = line.split(',');


    QString vers = words[1].replace('.', '0');
    int fileVersion = vers.toInt();

    QString assemblyVersion = QCoreApplication::applicationVersion();
    assemblyVersion = assemblyVersion.replace('.', '0');
    int appVersion = assemblyVersion.toInt();

    appVersion /= 100;
    fileVersion /= 100;

    if (fileVersion < appVersion)
    {
        qDebug() << "Saved tool file " << filename << " is in an older format and cannot be read.";
        //TODO error message
        return false;
    }
    else
    {
        line = reader.readLine(); words = line.split(',');
        //using system language settings for now
        //Properties.Settings.Default.setF_culture = (words[1]);
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_CAMPITCH(words[0].toDouble());

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISBATMANON(true);
        else
            SETTINGS_SET_DISPLAY_ISBATMANON(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_LIGHTBARCMPP(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_LINEWIDTH(words[1].toInt());

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_COMPASS(true);
        else
            SETTINGS_SET_DISPLAY_COMPASS(false);
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_SHOWGRID(true);
        else
            SETTINGS_SET_DISPLAY_SHOWGRID(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_LIGHTBARON(true);
        else
            SETTINGS_SET_DISPLAY_LIGHTBARON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_LOGNMEA(true);
        else
            SETTINGS_SET_GPS_LOGNMEA(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISMETRIC(true);
        else
            SETTINGS_SET_DISPLAY_ISMETRIC(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_OGLZOOM(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISPUREON(true);
        else
            SETTINGS_SET_DISPLAY_ISPUREON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_SIDEGUIDELINES(true);
        else
            SETTINGS_SET_DISPLAY_SIDEGUIDELINES(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_SIM_ON(true);
        else
            SETTINGS_SET_SIM_ON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_SKYON(true);
        else
            SETTINGS_SET_DISPLAY_SKYON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_SPEEDO(true);
        else
            SETTINGS_SET_DISPLAY_SPEEDO(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_UTURNALWAYSON(true);
        else
            SETTINGS_SET_DISPLAY_UTURNALWAYSON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISAUTODAYNIGHT(true);
        else
            SETTINGS_SET_DISPLAY_ISAUTODAYNIGHT(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_FULLSCREEN(true);
        else
            SETTINGS_SET_DISPLAY_FULLSCREEN(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_EXPECTRTK(true);
        else
            SETTINGS_SET_GPS_EXPECTRTK(false);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPCASTERIP(words[1]);
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPCASTERPORT(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPCASTERURL(words[1]);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_NTRIPGGAMANUAL(true);
        else
            SETTINGS_SET_GPS_NTRIPGGAMANUAL(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_NTRIPON(true);
        else
            SETTINGS_SET_GPS_NTRIPON(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_GPS_NTRIPTCP(true);
        else
            SETTINGS_SET_GPS_NTRIPTCP(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPMANUALLAT(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPMANUALLON(words[1].toDouble());

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPMOUNT(words[1]);
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPGGAINTERVAL(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPSENDTOUDPPORT(words[1].toInt());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPUSERNAME(words[1]);
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_GPS_NTRIPPASSWORD(words[1]);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
        {} //SETTINGS_SET_??? UDP(true);
        else
        {} //SETTINGS_SET_?.toInt(); UDP(false);

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_SIM_LATITUDE(words[1].toDouble());
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_SIM_LONGITUDE(words[1].toDouble());

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_FIELDCOLORDAY(colorSettingStringToInt(words[1]));
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_SECTIONSCOLORDAY(colorSettingStringToInt(words[1]));
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_FIELDCOLORNIGHT(colorSettingStringToInt(words[1]));
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_SECTIONSCOLORNIGHT(colorSettingStringToInt(words[1]));
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_COLORDAY(colorSettingStringToInt(words[1]));
        line = reader.readLine(); words = line.split(',');
        SETTINGS_SET_DISPLAY_COLORDAY(colorSettingStringToInt(words[1]));

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISSIMPLE(true);
        else
            SETTINGS_SET_DISPLAY_ISSIMPLE(false);

        line = reader.readLine(); words = line.split(',');
        if(words[1].toLower().trimmed() == "true")
            SETTINGS_SET_DISPLAY_ISDAYMODE(true);
        else
            SETTINGS_SET_DISPLAY_ISDAYMODE(false);

        line = reader.readLine(); words = line.split(',');
        //TODO: Properties.Settings.Default.setDisplay_customColors = line.Substring(13);

        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();
        line = reader.readLine();

        //fill in the current variables with restored data
        QString envFileName = QFileInfo(filename).baseName();
        SETTINGS_SET_ENVIRONMENT_NAME(envFileName);

    }

    return true;


}

void FormGPS::fileOpenField(QString fieldDir)
{
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + fieldDir;

    QString filename = directoryName + "/Field.txt";

    QFile fieldFile(filename);
    if (!fieldFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open field " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    QTextStream reader(&fieldFile);

    //close the existing job and reset everything
    jobClose();

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
    pn.utmEast = offs[0].toInt();
    pn.utmNorth = offs[1].toInt();
    pn.zone = offs[2].toInt();

    //create a new grid
    worldGrid.createWorldGrid(pn.actualNorthing - pn.utmNorth, pn.actualEasting - pn.utmEast);

    //convergence angle update
    if (!reader.atEnd())
    {
        line = reader.readLine(); //Convergence
        line = reader.readLine();
        pn.convergenceAngle = line.toDouble();
        //TODO lblConvergenceAngle.Text = Math.Round(glm.toDegrees(pn.convergenceAngle), 3).ToString();
    }

    //start positions
    if (!reader.atEnd())
    {
        line = reader.readLine();
        line = reader.readLine();
        offs = line.split(',');

        pn.latStart = offs[0].toDouble();
        pn.lonStart = offs[1].toDouble();
    }

    fieldFile.close();


    // ABLine -------------------------------------------------------------------------------------------------
    fileLoadABLines();

    if (ABLine.lineArr.count() > 0)
    {
        ABLine.numABLineSelected = 1;
        ABLine.refPoint1 = ABLine.lineArr[ABLine.numABLineSelected - 1].origin;
        //ABLine.refPoint2 = ABLine.lineArr[ABLine.numABLineSelected - 1].ref2;
        ABLine.abHeading = ABLine.lineArr[ABLine.numABLineSelected - 1].heading;
        ABLine.setABLineByHeading(ABLine.abHeading);
        ABLine.isABLineSet = false;
        ABLine.isABLineLoaded = true;
    }
    else
    {
        ABLine.isABLineSet = false;
        ABLine.isABLineLoaded = false;
    }


    //CurveLines
    fileLoadCurveLines();
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
    filename = directoryName + "/Sections.txt";

    QFile sectionsFile(filename);
    if (!sectionsFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open sections " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    reader.setDevice(&sectionsFile);
    bool isv3 = false;
    fd.workedAreaTotal = 0;
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

        tool.section[0].triangleList = QSharedPointer<TriangleList>( new TriangleList);
        tool.section[0].patchList.append(tool.section[0].triangleList);


        for (int v = 0; v < verts; v++)
        {
            line = reader.readLine();
            QStringList words = line.split(',');
            vecFix.setX(words[0].toDouble());
            vecFix.setY(words[1].toDouble());
            vecFix.setZ(words[2].toDouble());
            tool.section[0].triangleList->append(vecFix);
        }

        //calculate area of this patch - AbsoluteValue of (Ax(By-Cy) + Bx(Cy-Ay) + Cx(Ay-By)/2)
        verts -= 2;
        if (verts >= 2)
        {
            for (int j = 1; j < verts; j++)
            {
                double temp = 0;
                temp = (*tool.section[0].triangleList)[j].x() * ((*tool.section[0].triangleList)[j + 1].y() - (*tool.section[0].triangleList)[j + 2].y()) +
                         (*tool.section[0].triangleList)[j + 1].x() * ((*tool.section[0].triangleList)[j + 2].y() - (*tool.section[0].triangleList)[j].y()) +
                             (*tool.section[0].triangleList)[j + 2].x() * ((*tool.section[0].triangleList)[j].y() - (*tool.section[0].triangleList)[j + 1].y());

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

    // Contour points ----------------------------------------------------------------------------

    filename = directoryName + "/Contour.txt";

    QFile contourFile(filename);
    if (!contourFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open contour " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    reader.setDevice(&contourFile);

    //read header
    line = reader.readLine();

    while (!reader.atEnd())
    {
        //read how many vertices in the following patch
        line = reader.readLine();
        int verts = line.toInt();

        Vec3 vecFix(0, 0, 0);

        ct.ptList = QSharedPointer<QVector<Vec3>>(new QVector<Vec3>);
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

    // Flags -------------------------------------------------------------------------------------------------

    //Either exit or update running save
    filename = directoryName + "/Flags.txt";

    QFile flagsFile(filename);
    if (!flagsFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open flags " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

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

    //Boundaries
    //Either exit or update running save
    filename = directoryName + "/Boundary.txt";

    QFile boundariesFile(filename);
    if (!boundariesFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open boundaries " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    reader.setDevice(&boundariesFile);
    //read header
    line = reader.readLine();//Boundary

    for (int k = 0; true; k++)
    {
        if (reader.atEnd()) break;

        bnd.bndArr.append(CBoundaryLines());
        turn.turnArr.append(CTurnLines());
        gf.geoFenceArr.append(CGeoFenceLines());

        //True or False OR points from older boundary files
        line = reader.readLine();

        //Check for older boundary files, then above line string is num of points
        if (line == "True")
        {
            bnd.bndArr[k].isDriveThru = true;
            line = reader.readLine();
        } else if (line == "False")
        {
            bnd.bndArr[k].isDriveThru = false;
            line = reader.readLine(); //number of points
        }

        //Check for latest boundary files, then above line string is num of points
        if (line == "True")
        {
            bnd.bndArr[k].isDriveAround = true;
            line = reader.readLine(); //number of points
        } else if( line == "False")
        {
            bnd.bndArr[k].isDriveAround = false;
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
                bnd.bndArr[k].bndLine.append(vecPt);
            }

            bnd.bndArr[k].calculateBoundaryArea();
            bnd.bndArr[k].preCalcBoundaryLines();
            if (bnd.bndArr[k].area > 0) bnd.bndArr[k].isSet = true;
            else bnd.bndArr[k].isSet = false;
        }
        else
        {
            bnd.bndArr.removeAt(bnd.bndArr.count() - 1);
            turn.turnArr.removeAt(bnd.bndArr.count() - 1);
            gf.geoFenceArr.removeAt(bnd.bndArr.count() - 1);
            k = k - 1;
        }
        if (reader.atEnd()) break;
    }

    calculateMinMax();
    turn.buildTurnLines(bnd, fd);
    gf.buildGeoFenceLines(bnd);
    mazeGrid.buildMazeGridArray(bnd,gf, minFieldX, maxFieldX, minFieldY, maxFieldY);

    flagsFile.close();

    // Headland  -------------------------------------------------------------------------------------------------
    filename = directoryName + "/Headland.txt";

    QFile headlandFile(filename);
    if (!headlandFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open headland " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

    reader.setDevice(&headlandFile);

    //read header
    line = reader.readLine();

    for (int k = 0; true; k++)
    {
        if (reader.atEnd()) break;

        hd.headArr[0].hdLine.clear();

        //read the number of points
        line = reader.readLine();
        int numPoints = line.toInt();

        if (numPoints > 0 && bnd.bndArr.count() >= hd.headArr.count())
        {

            hd.headArr[k].hdLine.clear();
            hd.headArr[k].calcList.clear();

            //load the line
            for (int i = 0; i < numPoints; i++)
            {
                line = reader.readLine();
                QStringList words = line.split(',');
                Vec3 vecPt(words[0].toDouble(),
                           words[1].toDouble(),
                           words[2].toDouble());
                hd.headArr[k].hdLine.append(vecPt);

                if (gf.geoFenceArr[0].isPointInGeoFenceArea(vecPt)) hd.headArr[0].isDrawList.append(true);
                else hd.headArr[0].isDrawList.append(false);
            }
            hd.headArr[k].preCalcHeadLines();
        }
    }

    //if (hd.headArr[0].hdLine.count() > 0) hd.isOn = true;
    hd.isOn = false;

    //if (hd.isOn) btnHeadlandOnOff.Image = Properties.Resources.HeadlandOn;
    //TODO: btnHeadlandOnOff.Image = Properties.Resources.HeadlandOff;

    headlandFile.close();

    //Recorded Path
    filename = directoryName + "/RecPath.txt";

    QFile recpathFile(filename);
    if (!recpathFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open recpath " << filename << "for reading!";
        //TODO timed messagebox
        return;
    }

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
    recpathFile.close();
}

void FormGPS::fileCreateField()
{
    if( ! isJobStarted)
    {
        qDebug() << "field not open";
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

    myFilename = directoryName + "/Field.txt";
    QFile fieldFile(myFilename);
    if (!fieldFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for writing!";
        return;
    }

    QTextStream writer(&fieldFile);

    QDateTime now = QDateTime::currentDateTime();

    //Write out the date
    writer << now.toString("yyyy-MMMM-dd hh:mm:ss tt") << endl;

    writer << "$FieldDir" << endl;
    writer << currentFieldDirectory << endl;

    //write out the easting and northing Offsets
    writer << "$Offsets" << endl;
    writer << pn.utmEast << "," << pn.utmNorth << "," << pn.zone << endl;

    writer << "Convergence" << endl;
    writer << pn.convergenceAngle << endl;

    writer << "StartFix" << endl;
    writer << pn.latitude << "," << pn.longitude << endl;

    fieldFile.close();
}

void FormGPS::fileCreateElevation()
{
    //Why is this the same as field.txt?

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

    myFilename = directoryName + "/Elevation.txt";
    QFile fieldFile(myFilename);
    if (!fieldFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << myFilename << "for writing!";
        return;
    }

    QTextStream writer(&fieldFile);

    QDateTime now = QDateTime::currentDateTime();

    //Write out the date
    writer << now.toString("yyyy-MMMM-dd hh:mm:ss tt") << endl;

    writer << "$FieldDir" << endl;
    writer << currentFieldDirectory << endl;

    //write out the easting and northing Offsets
    writer << "$Offsets" << endl;
    writer << pn.utmEast << "," << pn.utmNorth << "," << pn.zone << endl;

    writer << "Convergence" << endl;
    writer << pn.convergenceAngle << endl;

    writer << "StartFix" << endl;
    writer << pn.latitude << "," << pn.longitude << endl;

    fieldFile.close();
}

void FormGPS::fileSaveSections()
{
    if (tool.patchSaveList.count() == 0) return;

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/Sections.txt";
    QFile sectionFile(myFilename);
    if (!sectionFile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }

    QTextStream writer(&sectionFile);

    //for each patch, write out the list of triangles to the file
    QSharedPointer<QVector<QVector3D>> triList;
    foreach(triList, tool.patchSaveList)
    {
        int count2 = triList->count();
        writer << count2 << endl;

        for (int i=0; i < count2; i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << (*triList)[i].x() << "," << (*triList)[i].y()
                   << "," << (*triList)[i].z() << endl;
        }
    }

    //clear out that patchList and begin adding new ones for next save
    tool.patchSaveList.clear();
    sectionFile.close();
}

void FormGPS::fileCreateSections()
{
    //not needed. fileSaveSections() will create the file for us.
    //no longer using $Sections header

}

void FormGPS::fileCreateFlags()
{

}

void FormGPS::fileCreateContour()
{

}

void FormGPS::fileSaveContour()
{
    if (contourSaveList.count() == 0) return;

    QString myFilename;

    //get the directory and make sure it exists, create if not

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Fields/" + currentFieldDirectory;

    myFilename = directoryName + "/Contour.txt";
    QFile contourFile(myFilename);
    if (!contourFile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << myFilename << "for appending!";
        return;
    }

    QTextStream writer(&contourFile);
    QSharedPointer<QVector<Vec3>> triList;

    foreach (triList, contourSaveList)
    {
        int count2 = triList->count();

        writer << count2 << endl;

        for (int i = 0; i < count2; i++)
        {
            writer << qSetRealNumberPrecision(3)
                   << (*triList)[i].easting << ","
                   << (*triList)[i].northing << ","
                   << (*triList)[i].heading << endl;
        }
    }

    contourSaveList.clear();
    contourFile.close();
}

void FormGPS::fileSaveBoundary()
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

    QString filename = directoryName + "/Boundary.txt";

    QFile boundfile(filename);
    if (!boundfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&boundfile);
    writer << "$Boundary" << endl;
    for(int i = 0; i < bnd.bndArr.count(); i++)
    {
        writer << (bnd.bndArr[i].isDriveThru ? "True" : "False") << endl;
        writer << (bnd.bndArr[i].isDriveAround ? "True" : "False") << endl;
        //writer.WriteLine(bnd.bndArr[i].isOwnField);

        writer << bnd.bndArr[i].bndLine.count() << endl;
        if (bnd.bndArr[i].bndLine.count() > 0)
        {
            for (int j = 0; j < bnd.bndArr[i].bndLine.count(); j++)
                writer << qSetRealNumberPrecision(3)
                       << bnd.bndArr[i].bndLine[j].easting << ","
                       << bnd.bndArr[i].bndLine[j].northing << ","
                       << qSetRealNumberPrecision(5)
                       << bnd.bndArr[i].bndLine[j].heading << endl;
        }
    }


    boundfile.close();

}

void FormGPS::fileSaveHeadland()
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

    QString filename = directoryName + "/Headland.txt";

    QFile headfile(filename);
    if (!headfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&headfile);
    writer << "$Headland" << endl;
    for(int i = 0; i < hd.headArr.count(); i++)
    {
        writer << hd.headArr[i].hdLine.count() << endl;
        if (hd.headArr[i].hdLine.count() > 0)
        {
            for (int j = 0; j < hd.headArr[i].hdLine.count(); j++)
                writer << qSetRealNumberPrecision(3)
                       << hd.headArr[i].hdLine[j].easting << ","
                       << hd.headArr[i].hdLine[j].northing << ","
                       << qSetRealNumberPrecision(5)
                       << hd.headArr[i].hdLine[j].heading << endl;
        }
    }

    headfile.close();

}

void FormGPS::fileCreateRecPath()
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

    QString filename = directoryName + "/RecPath.txt";

    QFile recpathfile(filename);
    if (!recpathfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&recpathfile);

    writer << "$RecPath" << endl;
    writer << "0" << endl;

    recpathfile.close();

}

void FormGPS::fileSaveRecPath()
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

    QString filename = directoryName + "/RecPath.txt";

    QFile recpathfile(filename);
    if (!recpathfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&recpathfile);

    writer << "$RecPath" << endl;
    writer << recPath.recList.count() << endl;

    if (recPath.recList.count() > 0)
    {
        for (int j = 0; j < recPath.recList.count(); j++)
            writer << qSetRealNumberPrecision(3)
                   << recPath.recList[j].easting << ","
                   << recPath.recList[j].northing << ","
                   << recPath.recList[j].heading << ","
                   << qSetRealNumberPrecision(1)
                   << recPath.recList[j].speed << ","
                   << recPath.recList[j].autoBtnState << endl;

    }

    recpathfile.close();

}

void FormGPS::fileSaveFlags()
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

    QString filename = directoryName + "/Flags.txt";

    QFile flagsfile(filename);
    if (!flagsfile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&flagsfile);

    writer << "$Flags" << endl;

    int count2 = flagPts.count();
    writer << count2 << endl;

    for (int i = 0; i < count2; i++)
    {
        writer << flagPts[i].latitude << ","
               << flagPts[i].longitude << ","
               << flagPts[i].easting << ","
               << flagPts[i].northing << ","
               << flagPts[i].heading << ","
               << flagPts[i].color << ","
               << flagPts[i].ID << ","
               << flagPts[i].notes << endl;
    }

    flagsfile.close();

}

void FormGPS::fileSaveNMEA()
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

    QString filename = directoryName + "/NMEA_log.txt";

    QFile nmeafile(filename);
    if (!nmeafile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&nmeafile);

    writer << pn.logNMEASentence;

    pn.logNMEASentence.clear();

    nmeafile.close();
}

void FormGPS::fileSaveElevation()
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

    QString filename = directoryName + "/Elevation.txt";

    QFile elevfile(filename);
    if (!elevfile.open(QIODevice::Append))
    {
        qWarning() << "Couldn't open " << filename << "for writing!";
        return;
    }

    QTextStream writer(&elevfile);

    writer << sbFix;

    sbFix.clear();

    elevfile.close();
}

void FormGPS::fileSaveSingleFlagKML2(int flagNumber)
{

}

void FormGPS::fileSaveSingleFlagKML(int flagNumber)
{

}

void FormGPS::fileMakeKMLFromCurrentPosition(double lat, double lon)
{

}

void FormGPS::fileSaveFieldKML()
{

}
