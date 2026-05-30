#include "rf_smart_plug.h"

void rf_smart_plug() {
    // The CC1101 pins must be configured to an RF module to send RF signals.
    if (bruceConfigPins.rfModule != CC1101_SPI_MODULE) {
        displayError("Listener needs a CC1101!", true);
        return;
    }

    // Create RfCodes struct to create a test RF message to transmit.
    RfCodes txCode = RfCodes{};

    // Intertek ES1513U "On" command.
    txCode.frequency = 433920000;
    txCode.preset = "FuriHalSubGhzPresetOok270Async";
    txCode.protocol = "Princeton";
    txCode.Bit = 24;
    txCode.key = hexStringToDecimal("00 00 00 00 00 39 1A 1C");
    txCode.te = 154;

    // Always redraw on the first iteration to initialise the UI.
    bool redraw = true;

    // Loop until the app is exited by pressing escape.
    while (!check(EscPress)) {

        // Transmit the test message while the Select button is pressed,
        // preventing the banner from being cleared while the button is held.
        while (check(SelPress)) {
            // Display a banner when the command is being transmitted.
            String text = String("Sending Intertek ES1513U 'On' command...");
            displayRedStripe(text, getComplementaryColor2(bruceConfig.priColor), bruceConfig.priColor);

            // Transmit the RF command.
            sendRfCommand(txCode);

            // Flag the UI to be redrawn to clear the banner after the
            // transmission finishes to clear the banner.
            redraw = true;
        }

        // Check if the UI needs to be redrawn.
        if (redraw) {
            // Clear the redraw flag so the UI refresh is only triggered once.
            redraw = false;

            // Draw the border to clear the screen.
            drawMainBorder();
        }
    }
}
