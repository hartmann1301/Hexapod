#ifndef Motions_h
#define Motions_h

#include "Arduino.h"

#include "Position.h"
#include "Motion.h"

Position stand("stand");
Position standNeutral("standNeutral");
Position standForward("standForward");
Position standBackward("standBackward");
Position legsUp("legsUp");

Motion toStand("toStand", 1, MotionType::Sequence);
Motion toSit("toSit", 2, MotionType::Sequence);

Motion kickFront("kickFront", 2, MotionType::Sequence);
Motion kickFrontLeft("kickFrontLeft", 2, MotionType::Sequence);
Motion kickFrontRight("kickFrontRight", 2, MotionType::Sequence);
Motion kickSides("kickSides", 2, MotionType::Sequence);

Motion gaitTripod1("Tripod1", 6, MotionType::Gait);
Motion gaitTripod2("Tripod2", 6, MotionType::Gait);
Motion gaitSwing("Swing", 6, MotionType::Gait);

Motion rotateLeft("rotateLeft", 6, MotionType::Gait);
Motion rotateRight("rotateRight", 6, MotionType::Gait);

class Motions
{
public:
    static void init()
    {
        // create Positions
        stand.setKnees(HexapodLegs::all, Knee::stand);

        standNeutral.setHips(HexapodLegs::all, Hip::neutral);
        standNeutral.setKnees(HexapodLegs::all, Knee::stand);

        toStand.setHips(HexapodLegs::all, Hip::neutral);
        toStand.setKnees(HexapodLegs::all, Knee::stand);

        toSit.setHips(HexapodLegs::center, Hip::forward);
        toSit.next();
        toSit.setKnees(HexapodLegs::back, Knee::lowStep);

        kickFront.setHips(HexapodLegs::front, Hip::forward);
        kickFront.next();
        kickFront.setKnees(HexapodLegs::front, Knee::up);

        kickFrontLeft.setHips(HexapodLegs::leftFront, Hip::backward);
        kickFrontLeft.next();
        kickFrontLeft.setKnees(HexapodLegs::leftFront, Knee::up);

        kickFrontRight.setHips(HexapodLegs::rightFront, Hip::backward);
        kickFrontRight.next();
        kickFrontRight.setKnees(HexapodLegs::rightFront, Knee::up);

        kickSides.setKnees(HexapodLegs::center, Knee::up);
        kickSides.next();
        kickSides.setHips(HexapodLegs::center, Hip::forward);

        /* std move forward and backward gait
        walk.setKnees(HexapodLegs::tripodLeft, Knee::midStep);
        walk.next();
        walk.setHips(HexapodLegs::tripodLeft, Hip::forward);
        walk.setHips(HexapodLegs::tripodRight, Hip::backward);
        walk.next();
        walk.setKnees(HexapodLegs::tripodLeft, Knee::stand);
        walk.next();
        walk.setKnees(HexapodLegs::tripodRight, Knee::midStep);
        walk.next();
        walk.setHips(HexapodLegs::tripodRight, Hip::forward);
        walk.setHips(HexapodLegs::tripodLeft, Hip::backward);
        walk.next();
        walk.setKnees(HexapodLegs::tripodRight, Knee::stand);
        */

        setStepHeight(Knee::midStep);

        gaitSwing.resetCount();
        // really weird gaitSwing
        gaitSwing.setHips(HexapodLegs::front, Hip::forward);
        gaitSwing.setHips(HexapodLegs::back, Hip::backward);
        
        gaitSwing.setKnees(HexapodLegs::front, Knee::standBodyMax); 
        gaitSwing.setKnees(HexapodLegs::back, Knee::standAwayMax);
        gaitSwing.setHips(HexapodLegs::center, Hip::forwardMax);
        gaitSwing.next();
        gaitSwing.setKnees(HexapodLegs::center, Knee::stand); // vorne entlasten
        gaitSwing.next();
        gaitSwing.setKnees(HexapodLegs::front, Knee::standAwayMax); // vorne vorziehen
        gaitSwing.next();
        gaitSwing.setHips(HexapodLegs::front, Hip::forward);
        gaitSwing.setHips(HexapodLegs::back, Hip::backward);

        gaitSwing.setHips(HexapodLegs::center, Hip::backwardMax); // kippen und hinten entlasten
        gaitSwing.next();
        gaitSwing.setKnees(HexapodLegs::back, Knee::standBodyMax); // hinten vorziehen
        gaitSwing.next();
        gaitSwing.setKnees(HexapodLegs::center, Knee::highStep); // seitliche beine wegziehen
    }

    static void setStepHeight(Knee stepHeight)
    {
        gaitTripod1.resetCount();
        // std move forward and backward gait
        gaitTripod1.setKnees(HexapodLegs::tripodLeft, stepHeight);
        gaitTripod1.next();
        // tripod left forward
        gaitTripod1.setHips(HexapodLegs::leftFront, Hip::neutral);
        gaitTripod1.setHips(HexapodLegs::leftBack, Hip::forwardMax);        
        gaitTripod1.setHips(HexapodLegs::rightCenter, Hip::forward);
        // tripod right backward
        gaitTripod1.setHips(HexapodLegs::rightFront, Hip::backwardMax);
        gaitTripod1.setHips(HexapodLegs::rightBack, Hip::neutral);        
        gaitTripod1.setHips(HexapodLegs::leftCenter, Hip::backward);
        gaitTripod1.next();
        gaitTripod1.setKnees(HexapodLegs::tripodLeft, Knee::stand);
        gaitTripod1.next();
        gaitTripod1.setKnees(HexapodLegs::tripodRight, stepHeight);
        gaitTripod1.next();
        // tripod left backward
        gaitTripod1.setHips(HexapodLegs::leftFront, Hip::backwardMax);
        gaitTripod1.setHips(HexapodLegs::leftBack, Hip::neutral);        
        gaitTripod1.setHips(HexapodLegs::rightCenter, Hip::backward);        
        // tripod right forward
        gaitTripod1.setHips(HexapodLegs::rightFront, Hip::neutral);
        gaitTripod1.setHips(HexapodLegs::rightBack, Hip::forwardMax);        
        gaitTripod1.setHips(HexapodLegs::leftCenter, Hip::forward);
        gaitTripod1.next();
        gaitTripod1.setKnees(HexapodLegs::tripodRight, Knee::stand);


        gaitTripod2.resetCount();
        // gait with extra knee movement to reduce feet grinding
        gaitTripod2.setKnees(HexapodLegs::tripodLeft, stepHeight);
        gaitTripod2.next();
        // tripod left forward
        gaitTripod2.setHips(HexapodLegs::leftFront, Hip::neutral);
        gaitTripod2.setHips(HexapodLegs::leftBack, Hip::forwardMax);  
        gaitTripod2.setHips(HexapodLegs::rightCenter, Hip::forward);
        // tripod right backward
        gaitTripod2.setHips(HexapodLegs::rightFront, Hip::backwardMax);
        gaitTripod2.setKnees(HexapodLegs::rightFront, Knee::standBody);
        gaitTripod2.setHips(HexapodLegs::rightBack, Hip::neutral);   
        gaitTripod2.setKnees(HexapodLegs::rightBack, Knee::standAway);        
        gaitTripod2.setHips(HexapodLegs::leftCenter, Hip::backward);
        gaitTripod2.next();
        gaitTripod2.setKnees(HexapodLegs::leftFront, Knee::standAway);
        gaitTripod2.setKnees(HexapodLegs::leftBack, Knee::standBody);        
        gaitTripod2.setKnees(HexapodLegs::rightCenter, Knee::stand);
        gaitTripod2.next();
        gaitTripod2.setKnees(HexapodLegs::tripodRight, stepHeight);
        gaitTripod2.next();
        // tripod left backward
        gaitTripod2.setHips(HexapodLegs::leftFront, Hip::backwardMax);
        gaitTripod2.setKnees(HexapodLegs::leftFront, Knee::standBody);
        gaitTripod2.setHips(HexapodLegs::leftBack, Hip::neutral);
        gaitTripod2.setKnees(HexapodLegs::leftBack, Knee::standAway);     
        gaitTripod2.setHips(HexapodLegs::rightCenter, Hip::backward);        
        // tripod right forward
        gaitTripod2.setHips(HexapodLegs::rightFront, Hip::neutral);
        gaitTripod2.setHips(HexapodLegs::rightBack, Hip::forwardMax);      
        gaitTripod2.setHips(HexapodLegs::leftCenter, Hip::forward);
        gaitTripod2.next();
        gaitTripod2.setKnees(HexapodLegs::rightFront, Knee::standAway);
        gaitTripod2.setKnees(HexapodLegs::rightBack, Knee::standBody);        
        gaitTripod2.setKnees(HexapodLegs::leftCenter, Knee::stand);


        rotateLeft.resetCount();
        // std rotateLeft left and right gait
        rotateLeft.setKnees(HexapodLegs::tripodLeft, Knee::midStep); // left in air
        rotateLeft.next();
        rotateLeft.setHips(HexapodLegs::leftOutside, Hip::backward);
        rotateLeft.setHips(HexapodLegs::leftCenter, Hip::forward);
        rotateLeft.setHips(HexapodLegs::rightOutside, Hip::backward);
        rotateLeft.setHips(HexapodLegs::rightCenter, Hip::forward);
        rotateLeft.next();
        rotateLeft.setKnees(HexapodLegs::tripodLeft, Knee::stand); // left on ground
        rotateLeft.next();
        rotateLeft.setKnees(HexapodLegs::tripodRight, Knee::midStep); // right in air
        rotateLeft.next();
        rotateLeft.setHips(HexapodLegs::leftOutside, Hip::forward);
        rotateLeft.setHips(HexapodLegs::leftCenter, Hip::backward);
        rotateLeft.setHips(HexapodLegs::rightOutside, Hip::forward);
        rotateLeft.setHips(HexapodLegs::rightCenter, Hip::backward);
        rotateLeft.next();
        rotateLeft.setKnees(HexapodLegs::tripodRight, Knee::stand); // right on ground

        rotateRight.resetCount();
        // std rotateLeft left and right gait
        rotateRight.setKnees(HexapodLegs::tripodLeft, Knee::midStep); // left in air
        rotateRight.next();
        rotateRight.setHips(HexapodLegs::leftOutside, Hip::forward);
        rotateRight.setHips(HexapodLegs::leftCenter, Hip::backward);
        rotateRight.setHips(HexapodLegs::rightOutside, Hip::forward);
        rotateRight.setHips(HexapodLegs::rightCenter, Hip::backward);
        rotateRight.next();
        rotateRight.setKnees(HexapodLegs::tripodLeft, Knee::stand); // left on ground
        rotateRight.next();
        rotateRight.setKnees(HexapodLegs::tripodRight, Knee::midStep); // right in air
        rotateRight.next();
        rotateRight.setHips(HexapodLegs::leftOutside, Hip::backward);
        rotateRight.setHips(HexapodLegs::leftCenter, Hip::forward);
        rotateRight.setHips(HexapodLegs::rightOutside, Hip::backward);
        rotateRight.setHips(HexapodLegs::rightCenter, Hip::forward);
        rotateRight.next();
        rotateRight.setKnees(HexapodLegs::tripodRight, Knee::stand); // right on ground
    }

private:
};

#endif