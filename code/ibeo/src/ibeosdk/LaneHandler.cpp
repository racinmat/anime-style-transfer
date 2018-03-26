//======================================================================
/*! \file LaneHandler.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief LaneHandler which implements functionality for the CarriageWay / Lane
 *        linked lists
 *///-------------------------------------------------------------------

#include <ibeosdk/LaneHandler.hpp>

//======================================================================

namespace ibeosdk{
namespace lanes{

//=============================================================================

LaneHandler::LaneHandler(const CarriageWayList& ways) :
	m_ways(ways)
{
	for (CarriageWayList::const_iterator it = m_ways.begin(); it != m_ways.end(); it++)
	{
		(*it)->resolveConnections(*it);
	}
}

//=============================================================================

LaneHandler::LaneHandler()
  : m_ways()
{}

//=============================================================================

LanePosition LaneHandler::getLanePosition(const VehicleStateBasicEcu2808& point) const
{
	PositionWgs84 p;
	p.transformFromTangentialPlane2d(point.getXPosition(), point.getYPosition(), point.getReferencePoint());
	p.setCourseAngleInRad(point.getCourseAngle());

	return getLanePosition(p);
}

//=============================================================================

LanePosition LaneHandler::getLanePosition(const PositionWgs84& point) const
{
	LanePosition out;
	out.gpsPosition = point;

	//======================================
	// find carriage ways with valid bounding boxes for point being inside
	CarriageWayList validWays;
	for (CarriageWayList::const_iterator it = m_ways.begin(); it != m_ways.end(); ++it) {
		BoundingRectangle mBox = (*it)->getBoundingBox();
		if (mBox.checkInside(point))
			validWays.push_back((*it));
	}

	// return if no way was found
	if (validWays.empty())
		return out;

	//======================================
	// search valid CarriageWaySegments
	std::vector<CarriageWaySegmentPtr> validCWSs;
	for (std::vector<CarriageWayPtr>::const_iterator it = validWays.begin(); it != validWays.end(); ++it) {
		CarriageWaySegmentMap::const_iterator segIt = (*it)->getSegments().begin();
		for (;segIt != (*it)->getSegments().end(); ++segIt) {
			CarriageWaySegmentPtr seg = segIt->second;
			BoundingRectangle mBox = segIt->second->getBoundingBox();

			if (mBox.checkInside(point))
				validCWSs.push_back(segIt->second);
		}
	}

	// return if no way segment was found
	if (validCWSs.size() == 0)
		return out;

	//======================================
	// search valid Lanes
	std::vector<LanePtr> validLanes;
	std::vector<CarriageWaySegmentPtr>::iterator cwspIter = validCWSs.begin();
	for (; cwspIter != validCWSs.end(); ++cwspIter) {
		LaneMap::const_iterator laneIt = (*cwspIter)->getLanes().begin();
		for (; laneIt != (*cwspIter)->getLanes().end(); ++laneIt) {
			BoundingRectangle mBox = laneIt->second->getBoundingBox();

			if (mBox.checkInside(point))
				validLanes.push_back(laneIt->second);
		}
	}

	// return if no lane was found
	if (validLanes.size() == 0)
		return out;

	//======================================
	// search valid Lane Segments
	std::vector<LaneSegmentPtr> validLaneSegments;
	std::vector<LanePtr>::iterator lpIter = validLanes.begin();
	for (; lpIter != validLanes.end(); ++lpIter) {
		LaneSegmentMap::const_iterator segIt = (*lpIter)->getSegments().begin();
		for (; segIt != (*lpIter)->getSegments().end(); ++segIt) {
			const BoundingRectangle mBox = segIt->second->getBoundingBox();

			if (mBox.checkInside(point))
				validLaneSegments.push_back(segIt->second);
		}
	}

	// return if no lane segment was found
	if (validLaneSegments.size() == 0)
		return out;

	//======================================
	// now find the most suitable segment
	LanePosition bestInside, bestOutside;
	float minOutsideDist = .0f;
	bool bi = false, bo = false;
	for (std::vector<LaneSegmentPtr>::iterator it = validLaneSegments.begin(); it != validLaneSegments.end(); it++)
	{
		LaneSegmentPtr curSeg = *it;
		bool inside = false;
		LanePosition curLP = calculateLanePosition(curSeg, point, inside );

		// condiations: intersection of projection is inside lane AND
		//              lateral distance is smaller than half of lane width in projection point AND
		//              the distance is closer than a previous found candidate
		if (inside && ( std::abs(curLP.lateralDistance) <= (curLP.laneSegment->getWidth(curLP.longitudinalDistance)/2) + 0.001f) // add 1mm to avoid rounding errors
			&& (!bi || std::abs(curLP.lateralDistance) < std::abs(bestInside.lateralDistance)))
		{
			bestInside = curLP;
			bi = true;
		}
		else if (!inside)
		{
			float startDis = std::abs(curLP.longitudinalDistance);
			float endDis = std::abs(curLP.longitudinalDistance - curSeg->getLength());

			float useDis = startDis < endDis ? startDis : endDis;

			if (!bo || useDis < minOutsideDist)
		{
			bestOutside = curLP;
				minOutsideDist = useDis;
			bo = true;
		}
	}
	}

	if (bi)
		return bestInside;
	else if (bo)
		return bestOutside;

	return out;
}

//=============================================================================

LanePosition LaneHandler::calculateLanePosition(const LaneSegmentPtr& segment, const PositionWgs84& point, bool& insideIntersection) const
{
	double xs, ys;
	point.transformToTangentialPlaneWithHeading2d(segment->getStartPoint().getPoint(), xs, ys);
	Point2dFloat p = Point2dFloat(float(xs), float(ys));
	Line2dFloat line = Line2dFloat(Point2dFloat(), segment->getEndOffset());
	Point2dFloat lineDir = line.getDiff().normalized();
	Point2dFloat perpDir = Point2dFloat(-lineDir.getY(), lineDir.getX());
	Line2dFloat perp = Line2dFloat(p + perpDir *15 , p - perpDir * 15); // 15m in both directions

	Point2dFloat intersection;
	Line2dFloat::IntersectingType ist =  line.isIntersecting(perp, &intersection);

	insideIntersection = ist == Line2dFloat::LineIntersecting;

	LanePosition out;
	out.laneSegment = segment;
	out.transformationMatrix = TransformationMatrix2dFloat(
		float(point.getCourseAngleInRad() - segment->getStartPoint().getPoint().getCourseAngleInRad()), p );
	out.lateralDistance = p.getY();
	out.longitudinalDistance = p.getX();
	out.gpsPosition = point;

	return out;
}

//==============================================================================

LanePosition LaneHandler::getReferenceLanePosition(const LanePosition& reference, const TransformationMatrix2dFloat& relativePosition, bool& insideReferenceLane, bool& success) const
{
	LanePosition out;
	success = false;
	insideReferenceLane = false;

	PositionWgs84 objectPosition;
	objectPosition.transformFromTangentialPlaneWithHeading2d(relativePosition.getPosition().getX(), relativePosition.getPosition().getY(), reference.gpsPosition);
	objectPosition.setCourseAngleInRad(reference.gpsPosition.getCourseAngleInRad() + relativePosition.getRotation().getAngle());

	if (reference.laneSegment) {
		// extract all segments with the same lane id as the reference lane
		UINT64 refLaneId = reference.laneSegment->getParent()->getLaneId();
		std::vector<LaneSegmentPtr> validLaneSegments;

		CarriageWayList::const_iterator it_way = m_ways.begin();
		for (; it_way != m_ways.end(); ++it_way) {
			CarriageWaySegmentMap::const_iterator it_ws = (*it_way)->getSegments().begin();
			for (; it_ws != (*it_way)->getSegments().end(); ++it_ws) {
				LaneMap::const_iterator it_l = it_ws->second->getLanes().begin();
				for (; it_l != it_ws->second->getLanes().end(); ++it_l) {
					LaneSegmentMap::const_iterator it_ls = it_l->second->getSegments().begin();
					for (; it_ls != it_l->second->getSegments().end(); ++it_ls) {
						if (it_ls->second->getParent()->getLaneId() == refLaneId)
							validLaneSegments.push_back(it_ls->second);
					}
				}
			}
		}

		if (!validLaneSegments.empty()) {
			// find the best suitable
			LanePosition bestInside, bestOutside;
			float minOutsideDist = .0f;
			bool bi = false;
			bool bo = false;
			std::vector<LaneSegmentPtr>::const_iterator it = validLaneSegments.begin();
			for (; it != validLaneSegments.end(); ++it) {
				LaneSegmentPtr curSeg = *it;
				bool inside = false;
				LanePosition curLP = calculateLanePosition(curSeg, objectPosition, inside );

				if (inside && (!bi || std::abs(curLP.lateralDistance) < std::abs(bestInside.lateralDistance))) {
					bestInside = curLP;
					bi = true;
				}
				else if (!inside) {
					float startDis = std::abs(curLP.longitudinalDistance);
					float endDis = std::abs(curLP.longitudinalDistance - curSeg->getLength());

					float useDis = startDis < endDis ? startDis : endDis;

					if (!bo || useDis < minOutsideDist)
					{
						bestOutside = curLP;
						minOutsideDist = useDis;
						bo = true;
					}
				}
			}

			if (bi || bo) {
				LanePosition lP = bi ? bestInside : bestOutside;
				float length = bi ? bestInside.longitudinalDistance : 0;

				if (std::abs(lP.lateralDistance) <= lP.laneSegment->getWidth(length)/2)
					insideReferenceLane = true;


				// count length to front
				LaneSegmentPtr startSeg = reference.laneSegment;
				float lengthCount = 0;
				if (relativePosition.getPosition().getX() >= 0) {
					while (startSeg && lengthCount <= 300) {
						if (lP.laneSegment == startSeg)
						{
							lP.longitudinalDistance = lengthCount + lP.longitudinalDistance - reference.longitudinalDistance;
							success = true;
							return lP;
						}

						lengthCount += startSeg->getLength();
						startSeg = startSeg->getNext();
					}
				}
				else {
					while (startSeg && length >= -300) {
						if (lP.laneSegment == startSeg) {
							lP.longitudinalDistance = lengthCount + lP.longitudinalDistance - reference.longitudinalDistance;
							success = true;
							return lP;
						}

						startSeg = startSeg->getPrevious();
						if (!startSeg)
							break;
						lengthCount -= startSeg->getLength();
					} // while
				} // else
			} // if
		} // if
	} // if
	return out;
}

//==============================================================================

TransformationMatrix2dFloat LaneHandler::getRelativePosition(const LanePosition& reference, const float& longitudinalDistance, const float& lateralDistance, bool& success) const
{
	success = false;

	// find lane segment
	float ldistance = longitudinalDistance + reference.longitudinalDistance;
	LaneSegmentPtr startSeg = reference.laneSegment;

	if (ldistance > 0) {
		while (startSeg && ldistance > startSeg->getLength()) {
			ldistance -= startSeg->getLength();
			startSeg = startSeg->getNext();
		}
	}
	else {
		while (startSeg && ldistance < 0) {
			startSeg = startSeg->getPrevious();
			if (!startSeg)
				break;
			ldistance += startSeg->getLength();
		}
	}

	if (startSeg) {
		//create gps position for point
		PositionWgs84 objectPosition;
		objectPosition.transformFromTangentialPlaneWithHeading2d(ldistance, lateralDistance, startSeg->getStartPoint().getPoint());

		// create matrix relative to reference
		double x, y;
		objectPosition.transformToTangentialPlaneWithHeading2d(reference.gpsPosition, x, y);

		Point2dFloat position = Point2dFloat(float(x),float(y));
		float angle = float(objectPosition.getCourseAngleInRad() - reference.gpsPosition.getCourseAngleInRad());

		success = true;
		return TransformationMatrix2dFloat(angle, position);
	}
	else {
		success = false;
		return TransformationMatrix2dFloat();
	}
}

//==============================================================================

} // namespace lanes
} // namespace ibeosdk

//==============================================================================

