#include "control-form.hpp"
#include "ui_control-form.h"
#include "core/util.hpp"
#include <QDebug>

ControlForm::ControlForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlForm)
{
    ui->setupUi(this);
    // Frequency
    ui->doubleSpinBoxMutationFrequency->setValue(0.0f);
    ui->doubleSpinBoxMutationFrequency->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationFrequency->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationFrequency->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationFrequencyDown->setValue(0.0f);
    ui->doubleSpinBoxMutationFrequencyDown->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationFrequencyDown->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationFrequencyDown->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationFrequencyUp->setValue(0.0f);
    ui->doubleSpinBoxMutationFrequencyUp->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationFrequencyUp->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationFrequencyUp->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationFrequencyMax->setValue(1.0f);
    ui->doubleSpinBoxMutationFrequencyMax->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationFrequencyMax->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationFrequencyMax->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationFrequencyMin->setValue(0.0f);
    ui->doubleSpinBoxMutationFrequencyMin->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationFrequencyMin->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationFrequencyMin->setSingleStep(
                Util::getLineFromConf("step").toFloat());
    // Intensity
    ui->doubleSpinBoxMutationIntensity->setValue(0.0f);
    ui->doubleSpinBoxMutationIntensity->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationIntensity->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationIntensity->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationIntensityDown->setValue(0.0f);
    ui->doubleSpinBoxMutationIntensityDown->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationIntensityDown->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationIntensityDown->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationIntensityUp->setValue(0.0f);
    ui->doubleSpinBoxMutationIntensityUp->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationIntensityUp->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationIntensityUp->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationIntensityMax->setValue(1.0f);
    ui->doubleSpinBoxMutationIntensityMax->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationIntensityMax->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationIntensityMax->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    ui->doubleSpinBoxMutationIntensityMin->setValue(0.0f);
    ui->doubleSpinBoxMutationIntensityMin->setRange(0.0f, 1.0f);
    ui->doubleSpinBoxMutationIntensityMin->setDecimals(
                Util::getLineFromConf("decimals").toFloat());
    ui->doubleSpinBoxMutationIntensityMin->setSingleStep(
                Util::getLineFromConf("step").toFloat());

    //
    ui->doubleSpinBoxLimitDeviation->setValue(0.0f);
    ui->doubleSpinBoxLimitDeviation->setRange(0.0f, 10000.0f);
    ui->doubleSpinBoxLimitDeviation->setDecimals(2);
    ui->doubleSpinBoxLimitDeviation->setSingleStep(1);
}

ControlForm::~ControlForm()
{
    delete ui;
}
