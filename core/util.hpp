#pragma once

#define RESET      ""
#define LUMINEUX   "\033[1m"
#define LEGER      "\033[2m"
#define SOULIGNE   "\033[4m"
#define CLIGNOTANT "\033[5m"
#define INVERSE    "\033[7m"
#define CACHE      "\033[8m"

#define NOIR    "\033[30m"
#define ROUGE   ""
#define VERT    ""
#define JAUNE   ""
#define BLEU    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BLANC   "\033[37m"

#define FOND_NOIR       "\033[40m"
#define FOND_ROUGE      "\033[41m"
#define FOND_VERT       "\033[42m"
#define FOND_JAUNE      "\033[43m"
#define FOND_BLEU       "\033[44m"
#define FOND_MAGENTA    "\033[45m"
#define FOND_CYAN       "\033[46m"
#define FOND_BLANC      "\033[47m"

#include <QString>


class MainWindow;

class Util
{

public:

    static void init(MainWindow * mainWindow);
    static QString getLineFromConf(const QString &id);
    static float getRandomFloat(const float & min, const float & max);
    static int getRandomInt(const int & min, const int & max);

    static void write(const QString & message);
    static void writeSuccess(const QString & message);
    static void writeWarning(const QString & message);
    static void writeError(const QString & message);

    static void overwrite(const QString &message);
    static void overwriteSuccess(const QString & message);
    static void overwriteWarning(const QString & message);
    static void overwriteError(const QString & message);

private:

    static MainWindow * mainWindow;
    static QString configFileName;

    static bool writeEnabled;
    static bool overwriteEnabled;
    static bool successEnabled;
    static bool successOverwriteEnabled;
    static bool warningsEnabled;
    static bool warningOverwriteEnabled;
    static bool errorEnabled;
    static bool errorOverwriteEnabled;

    static bool isOverwriting;

};
