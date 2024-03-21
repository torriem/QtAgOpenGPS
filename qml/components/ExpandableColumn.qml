import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."

Column{
    id: columnRoot
    signal adjust(var adjustHeight)
    onAdjust:{
            var totalChildrenHeight = 0
            var numOfChildren = 0
            for (numOfChildren = 0; numOfChildren < children.length; numOfChildren++){
                totalChildrenHeight = children[numOfChildren].height + totalChildrenHeight
            }
            if(totalChildrenHeight > adjustHeight){
                columnRoot.spacing = 0
            }else{
                columnRoot.spacing = ((adjustHeight - totalChildrenHeight)/(numOfChildren-1))
            }
        }
}
