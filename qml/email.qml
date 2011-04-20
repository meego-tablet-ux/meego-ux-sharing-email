/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
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

      console.log("Launching email: " + shareCmd + ", shareType: " + sharingObj.shareType);
      spinnerContainer.startSpinner();
      appModel.launch(shareCmd);
      //Have to do it this way so that the parent class can actually
      //connect to the shared signal - otherwise it never gets it...
      signalTimer.start();

  }

  Item {
      id: spinnerContainer
      parent: scene.content
      anchors.fill: scene.content
      property variant overlay: null

      TopItem {
          id: topItem
      }

      Component {
          id: spinnerOverlayComponent
          Item {
              id: spinnerOverlayInstance
              anchors.fill: parent

              Connections {
                  target: qApp
                  onWindowListUpdated: {
                      spinnerOverlayInstance.destroy();
                  }
              }

              Rectangle {
                  anchors.fill: parent
                  color: "black"
                  opacity: 0.7
              }
              Spinner {
                  anchors.centerIn: parent
                  spinning: true
                  onSpinningChanged: {
                      if (!spinning)
                      {
                          spinnerOverlayInstance.destroy()
                      }
                  }
              }
              MouseArea {
                  anchors.fill: parent
                  // eat all mouse events
              }
          }
      }

      function startSpinner() {
          if (overlay == null)
          {
              overlay = spinnerOverlayComponent.createObject(spinnerContainer);
              overlay.parent = topItem.topItem;
          }
      }
  }

  Timer {
      id: signalTimer
      interval: 1000
      running: false
      repeat: false
      onTriggered: doShared()
  }

  ApplicationsModel {
    id: appModel
  }

  function doShared() {
      customArea.shared(-1);
  }
}
