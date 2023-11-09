#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

typedef void (*InitializeEngineFunc)();
typedef void (*UpdateEngineFunc)();

FILETIME lastWriteTime = {0};

bool isReloadTriggered() {

  WIN32_FILE_ATTRIBUTE_DATA attr;
  if (!GetFileAttributesEx("Engine.dll", GetFileExInfoStandard, &attr)) {

    printf("Failed to get file attributes: %lu\n", GetLastError());
    return false;
  }

  if (CompareFileTime(&lastWriteTime, &attr.ftLastWriteTime) != 0) {

    lastWriteTime = attr.ftLastWriteTime;
    return true;
  }

  return false;
}

int main() {

  WIN32_FILE_ATTRIBUTE_DATA attr;
  if (!GetFileAttributesEx("Engine.dll", GetFileExInfoStandard, &attr)) {

    printf("Failed to get file attributes: %lu\n", GetLastError());
    return 1;
  }
  lastWriteTime = attr.ftLastWriteTime;

  HMODULE hEngine = LoadLibrary("Engine.dll");
  if (!hEngine) {
    printf("Failed to load Engine.dll: %lu\n", GetLastError());
    return 1;
  }

  InitializeEngineFunc initializeEngine =
      (InitializeEngineFunc)GetProcAddress(hEngine, "InitializeEngine");
  UpdateEngineFunc updateEngine =
      (UpdateEngineFunc)GetProcAddress(hEngine, "UpdateEngine");

  if (!initializeEngine || !updateEngine) {
    printf("Failed to get function addresses: %lu\n", GetLastError());
    FreeLibrary(hEngine);
    return 1;
  }

  initializeEngine();

  bool gameIsRunning = true;
  while (gameIsRunning) {

    updateEngine();

    if (isReloadTriggered()) {

      FreeLibrary(hEngine);
      hEngine = LoadLibrary("Engine.dll");
      if (!hEngine) {
        printf("Failed to reload Engine.dll: %lu\n", GetLastError());
        return 1;
      }

      initializeEngine =
          (InitializeEngineFunc)GetProcAddress(hEngine, "InitializeEngine");
      updateEngine = (UpdateEngineFunc)GetProcAddress(hEngine, "UpdateEngine");
      if (!initializeEngine || !updateEngine) {
        printf("Failed to get function addresses: %lu\n", GetLastError());
        FreeLibrary(hEngine);
        return 1;
      }
    }
  }

  FreeLibrary(hEngine);

  return 0;
}
