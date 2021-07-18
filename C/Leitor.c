#include <stdlib.h>
#include <string.h>
#include <nfc/nfc.h>
int
CardTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen)
{
  int res;
  size_t  szPos;
  printf("=> ");
  for (szPos = 0; szPos < capdulen; szPos++) {
    printf("%02x ", capdu[szPos]);
  }
  printf("\n");
  if ((res = nfc_initiator_transceive_bytes(pnd, capdu, capdulen, rapdu, *rapdulen, 500)) < 0) {
    return -1;
  } else {
    *rapdulen = (size_t) res;
    printf("<= ");
    for (szPos = 0; szPos < *rapdulen; szPos++) {
      printf("%02x ", rapdu[szPos]);
    }
    printf("\n");
    return 0;
  }
}
int
main(int argc, const char *argv[])
{
  nfc_device *pnd;
  nfc_target nt;
  nfc_context *context;
  nfc_init(&context);
  if (context == NULL) {
    printf("Unable to init libnfc (malloc)\n");
    exit(EXIT_FAILURE);
  }
  const char *acLibnfcVersion = nfc_version();
  (void)argc;
  printf("%s uses libnfc %s\n", argv[0], acLibnfcVersion);

  pnd = nfc_open(context, NULL);

  if (pnd == NULL) {
    printf("ERROR: %s", "Unable to open NFC device.");
    exit(EXIT_FAILURE);
  }
  if (nfc_initiator_init(pnd) < 0) {
    nfc_perror(pnd, "nfc_initiator_init");
    exit(EXIT_FAILURE);
  }

  printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));

  const nfc_modulation nmMifare = {
    .nmt = NMT_ISO14443A,
    .nbr = NBR_106,
  };
  // nfc_set_property_bool(pnd, NP_AUTO_ISO14443_4, true);
  printf("Polling for target...\n");
  while (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) <= 0);
  printf("Target detected!\n");
  uint8_t capdu[264];
  size_t capdulen;
  uint8_t rapdu[264];
  size_t rapdulen;
  // Select application
  memcpy(capdu, "\x00\xA4\x04\x00\x07\xF0\x13\x13\x13\x13\x13\x13", 12);
  capdulen=12;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_SUCCESS);


  printf("\n");
  nfc_close(pnd);
  nfc_exit(context);
  exit(EXIT_SUCCESS);
}
