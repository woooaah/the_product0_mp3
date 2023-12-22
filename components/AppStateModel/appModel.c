#include <stdio.h>
#include "appModel.h"
#include "fileSystem.h"
#include "audioStream.h"
#include "BluetoothClassic/btClassic.h"

void hardware_init(void)
{
    fileSystem_init();
    btClassic_init();
}

void hardware_selectAudioInterface(audioInterface_t audioIf);

