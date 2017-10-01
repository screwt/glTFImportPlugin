/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __gLTFImportExecutor_H__
#define __gLTFImportExecutor_H__

#include <map>
#include "hlms_editor_plugin.h"
#include "gLTFImportConstants.h"
#include "gLTFMaterial.h"
#include "gLTFImportAccessorsParser.h"
#include "gLTFImportMeshesParser.h"
#include "gLTFImportMaterialsParser.h"
#include "gLTFImportTexturesParser.h"
#include "gLTFImportImagesParser.h"
#include "gLTFImportSamplersParser.h"
#include "gLTFImportBuffersParser.h"
#include "gLTFImportBufferViewsParser.h"
#include "gLTFImportPbsMaterialsCreator.h"
#include "gLTFImportOgreMeshCreator.h"
#include "rapidjson/document.h"

/** Class responsible for executing the import */
class gLTFImportExecutor
{
	public:
		gLTFImportExecutor (void) {};
		virtual ~gLTFImportExecutor(void) {};
		
		// Perform the import (called by plugin)
		bool executeImport(Ogre::HlmsEditorPluginData* data);

	protected:
		enum TextureTransformation
		{
			TTF_R_2_GB,
			TTF_G_2_RB,
			TTF_B_2_RG,
			TTF_R_2_GB_INV
		};

		// Process the binary file / text file
		bool executeBinary (const std::string& fileName, 
			Ogre::HlmsEditorPluginData* data,
			int& startBinaryBuffer); // proces .glb (binary) file
		bool executeText (const std::string& fileName, Ogre::HlmsEditorPluginData* data); // proces .gltf (json text) file
		bool executeJson (const std::string& fileName, 
			const char* jsonChar, 
			Ogre::HlmsEditorPluginData* data);

		// Data propagation (flatten the structure, so creation of Ogre 3d materials and meshes is less complex)
		bool propagateData (Ogre::HlmsEditorPluginData* data, 
			int startBinaryBuffer); // Arrange the data structure so it is easier to use when creating a Pbs material
		bool propagateBufferViews (void); // BufferViews are enriched with data from Buffers
		bool propagateMaterials (Ogre::HlmsEditorPluginData* data, int startBinaryBuffer); // Materials are enriched with data from BufferViews, Textures and Images
		bool propagateAccessors(void); // Accessors are enriched with data from Buffers and BufferViews
		bool propagateMeshes (Ogre::HlmsEditorPluginData* data, int startBinaryBuffer); // Primitives are enriched with data from Materials and Accessors
		
		// Utils
		const gLTFImage& getImageByTextureIndex (int index);
		int getImageIndexByTextureIndex (int index);
		const std::string& getImageUriByTextureIndex (int index);
		int getSamplerByTextureIndex (int index);
		const std::string& getMaterialNameByIndex (int index);

		// Copy / extract and texture conversion
		const std::string& copyImageFile (const std::string& textureName,
			Ogre::HlmsEditorPluginData* data, 
			const std::string& materialName,
			const std::string& uriImage);
		const std::string& extractAndCreateImageFromBufferByTextureIndex (const std::string& textureName,
			Ogre::HlmsEditorPluginData* data, 
			const std::string& materialName,
			int index,
			int startBinaryBuffer);
		bool convertTexture (const std::string& fileName, TextureTransformation transformation); // Convert an image file to a roughness file

	private:
		std::string mHelperString;
		std::string mHelperMaterialNameString;
		gLTFImage mHelperImage;
		std::map<int, gLTFAccessor> mAccessorsMap;
		std::map<int, gLTFMesh> mMeshesMap;
		std::map<std::string, gLTFMaterial> mMaterialsMap;
		std::map<int, gLTFTexture> mTexturesMap;
		std::map<int, gLTFImage> mImagesMap;
		std::map<int, gLTFSampler> mSamplersMap;
		std::map<int, gLTFBufferView> mBufferViewsMap;
		std::map<int, gLTFBuffer> mBuffersMap;

		gLTFImportAccessorsParser mAccessorsParser;
		gLTFImportMeshesParser mMeshesParser;
		gLTFImportMaterialsParser mMaterialsParser;
		gLTFImportTexturesParser mTexturesParser;
		gLTFImportSamplersParser mSamplersParser;
		gLTFImportBuffersParser mBuffersParser;
		gLTFImportBufferViewsParser mBufferViewsParser;
		gLTFImportImagesParser mImagesParser;
		
		// Creator classes
		gLTFImportPbsMaterialsCreator mPbsMaterialsCreator;
		gLTFImportOgreMeshCreator mOgreMeshCreator;
};

#endif
