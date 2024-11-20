import QtQuick
import QtQuick.Controls.Fusion

import ".."

Column{
    id: columnRoot
    property var adjustHeight: height
    onVisibleChanged: adjust(adjustHeight)
    function adjust(adjustHeight){
            var totalChildrenHeight = 0
            var numOfChildren = 0
            var numOfVisibleChildren = 0
            for (numOfChildren = 0; numOfChildren < children.length; numOfChildren++){
                if(children[numOfChildren].visible === true){
                    totalChildrenHeight = children[numOfChildren].height + totalChildrenHeight
                    numOfVisibleChildren++
                }
            }
            if(totalChildrenHeight > adjustHeight){
                columnRoot.spacing = 0
            }else{
                columnRoot.spacing = ((adjustHeight - totalChildrenHeight)/(numOfVisibleChildren-1))
            }
        }
}
