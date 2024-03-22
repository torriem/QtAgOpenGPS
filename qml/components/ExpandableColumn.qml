import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."

Column{
    id: columnRoot
    signal adjust(var adjustHeight)
    onVisibleChanged: adjust(adjust.adjustHeight)
    onAdjust:{
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
