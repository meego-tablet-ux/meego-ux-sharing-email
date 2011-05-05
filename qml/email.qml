/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Sharing 0.1

Item {
  id: customArea

  signal cancel()
  signal shared(int shareid)
  signal shareError(string errMsg)

  Component.onCompleted: {
      var shareCmd = "/usr/bin/meego-qml-launcher --opengl --app meego-app-email --fullscreen --cmd openComposer --cdata \"" + sharingObj.filesToShare + "\"";
      if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeText)
          shareCmd = "/usr/bin/meego-qml-launcher --opengl --app meego-app-email --fullscreen --cmd compose --cdata \";;"
              + sharingObj.getHashEntryForFile(sharingObj.filesToShare[0], "subject") + ";;"
              + sharingObj.filesToShare[0] + "\"";
      if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeVCard)
          shareCmd = shareCmd;
      //TODO: once meego-app-email supports doing a Compose open w/ attachment and a subject, modify the above shareCmd

      console.log("Launching email: " + shareCmd + ", shareType: " + sharingObj.shareType);
      spinner.show();
      appModel.launch(shareCmd);
      //Have to do it this way so that the parent class can actually
      //connect to the shared signal - otherwise it never gets it...
      signalTimer.start();

  }

  ModalSpinner {
      id: spinner
  }

  Timer {
      id: signalTimer
      interval: 6000
      running: false
      repeat: false
      onTriggered: doShared()
  }

  Labs.ApplicationsModel {
    id: appModel
  }

  function doShared() {
      spinner.hide()
      customArea.shared(-1);
  }
}
