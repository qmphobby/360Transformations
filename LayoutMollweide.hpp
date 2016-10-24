/************************
*Institute:XIDIAN MMC
*Author:MengPin Qiu
*Function£ºMollweide Projection(Equal-Area)
*Date:2016-10-11
************************/
#pragma once
#include "Layout.hpp"
#include <cmath>
namespace IMT {
	class LayoutMollweide : public Layout
	{
	public:
		LayoutMollweide(unsigned int width,unsigned int height,double yaw,double pitch,double roll):
			Layout(width,height),m_rotationMatrice(GetRotMatrice(yaw,pitch,roll)){}
		virtual ~LayoutMollweide(void) = default;

		virtual CoordI GetReferenceResolution(void) override
		{
			return CoordI(GetWidth(),GetHeight());
		}
		
	protected:
		virtual NormalizedFaceInfo From2dToNormalizedFaceInfo(const CoordI& pixel) const override;

		virtual CoordF FromNormalizedInfoTo2d(const NormalizedFaceInfo& ni) const override ;

		virtual NormalizedFaceInfo From3dToNormalizedFaceInfo(const Coord3dSpherical& sphericalCoord) const final ;

		virtual Coord3dCart FromNormalizedInfoTo3d(const NormalizedFaceInfo& ni) const final ;

		virtual std::shared_ptr<Picture> ReadNextPictureFromVideoImpl(void) override
		{
			auto matptr = m_inputVideoPtr->GetNextPicture(0);
			return std::make_shared<Picture>(matptr->clone());
		}

		virtual void WritePictureToVideoImpl(std::shared_ptr<Picture> pict) override
		{
			m_outputVideoPtr->Write(pict->GetMat(),0);
		}

		virtual std::shared_ptr<IMT::LibAv::VideoReader> InitInputVideoImpl(std::string pathToInputVideo, unsigned nbFrame) override
		{
			std::shared_ptr<IMT::LibAv::VideoReader> vrPtr = std::make_shared<IMT::LibAv::VideoReader> (pathToInputVideo);
			if (vrPtr->GetNbStream() != 1)
			{
				std::cout << "Unsupported number of stream for Equirectangular input video: " << vrPtr->GetNbStream() << " instead of 1" << std::endl;
				return nullptr;
			}
			return vrPtr;
		}

		virtual std::shared_ptr<IMT::LibAv::VideoWriter> InitOutputVideoImpl(std::string pathOutputVideo, std::string codecId, 
																			unsigned fps, unsigned gop_size, std::vector<unsigned> bit_rateVect)
		{
			std::shared_ptr<IMT::LibAv::VideoWriter> vwPtr = std::make_shared<IMT::LibAv::VideoWriter>(pathOutputVideo);
			vwPtr->Init<1>(codecId, { { m_outWidth } }, { { m_outHeight } }, fps, gop_size, { { bit_rateVect[0] } });
			return vwPtr;
		}

	private:
		RotMat m_rotationMatrice;
	};

}

