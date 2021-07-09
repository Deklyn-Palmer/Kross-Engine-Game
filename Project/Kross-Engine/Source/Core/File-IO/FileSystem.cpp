/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 *      - Chris Deitch
 */

#include "FileSystem.h"

#define MANIFEST_FILEPATH "manifest.krs"
#define LINE_DIVIDER "->"

/* For Writing the Files. */
#define ASSET_FILEPATH "Assets/"

#include "../Manager/ResourceManager.h"

namespace Kross
{
	std::string FileSystem::GetFileContents(const std::string& filepath)
	{
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;
			std::stringstream contents;

			/* Read the file line by line. */
			while (getline(fileStream, line))
				contents << line << "\n";

			fileStream.close();

			/* Return the file contents. */
			return contents.str();
		}

		else
		{
			fileStream.close();

			/* Return an empty string as it could not open the file. */
			return std::string();
		}
	}

	std::string FileSystem::GetFileContentsSingle(const std::string& filepath, int line)
	{
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string lineString;
			int lineIndex = 0;

			/* Read the file line by line. */
			while (getline(fileStream, lineString) && lineIndex < line)
				lineIndex++;

			fileStream.close();

			/* If the line index wasn't met then return a emtpy string. */
			if (lineIndex < line)
				return std::string();

			/* Return the file contents. */
			return lineString;
		}

		else
		{
			fileStream.close();

			/* Return an empty string as it could not open the file. */
			return std::string();
		}
	}

	void FileSystem::OnReadManifestFile()
	{
		/* The Filepath for the Mainfest. */
		std::string filepath = MANIFEST_FILEPATH;

		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* If the Line size is zero. */
				if (line.empty())
					continue;

				/* Just so It doesn't read "MANIFEST:". */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string assetType;
				std::string assetFilepath;
				std::string manifestSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(manifestSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Asset Type. */
					if (varSwitch == 0)
						assetType = line.substr(0, searchPosition);

					/* Grab the Asset Filepath. */
					else
						assetFilepath = line.substr(0, searchPosition);

					line.erase(0, searchPosition + manifestSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}

				/* Determain how it gets loaded in. */

				if (assetType == "SPRITE")
					OnReadSprite(assetFilepath);

				else if (assetType == "TEXTURE")
					OnReadTexture(assetFilepath);

				else if (assetType == "SHADER")
					OnReadShader(assetFilepath);

				else if (assetType == "FONT")
					OnReadFont(assetFilepath);

				else if (assetType == "MATERIAL")
					OnReadMaterial(assetFilepath);

				else if (assetType == "ANIMATION")
					OnReadAnimation(assetFilepath);

				else if (assetType == "AUDIOSOURCE")
					OnReadAudioSource(assetFilepath);

				else if (assetType == "TILEMAP")
					OnReadTileMap(assetFilepath);

				else if(assetType == "TILESET")
					OnReadTileSet(assetFilepath);
			}

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadTexture(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string textureFilepath;
		std::string textureName;
		std::string textureType;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Just so It doesn't read "TEXTURE:". */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string textureProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						textureProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (textureProperty == "NAME")
							textureName = line.substr(0, searchPosition);

						else if (textureProperty == "FILEPATH")
							textureFilepath = line.substr(0, searchPosition);

						else if (textureProperty == "TYPE")
							textureType = line.substr(0, searchPosition);
					}


					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			if (textureType != "DEFAULT")
			{
				if (textureType == "FONTMAP")
					Texture::OnCreate(textureFilepath, textureName, TextureType::FontMap);

				else if (textureType == "NORMALMAP")
					Texture::OnCreate(textureFilepath, textureName, TextureType::NormalMap);

				else if (textureType == "SPECULARMAP")
					Texture::OnCreate(textureFilepath, textureName, TextureType::SpecularMap);
			}

			else
				Texture::OnCreate(textureFilepath, textureName);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadTileMap(const std::string& filepath) 
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());
		std::string mapName;
		std::string mapRawDataFilepath;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool hasFirstLine = false;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Special Case where it reads the first line. */
				if (!hasFirstLine)
				{
					hasFirstLine = true;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string tileMapProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						tileMapProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (tileMapProperty == "NAME")
							mapName = line.substr(0, searchPosition);

						else if (tileMapProperty == "RAWDATA")
							mapRawDataFilepath = line.substr(0, searchPosition);

					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			fileStream.close();
		}

		/* Open Raw Data. */
		fileStream.open(mapRawDataFilepath);

		List<List<int>> dataConverted;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{

				/* Quick Variables. */
				size_t searchPosition = 0;
				List<std::string> data;
				std::string lineSplitter = ",";

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos)
				{
					data.push_back(line.substr(0, searchPosition));

					line.erase(0, searchPosition + lineSplitter.length());
				}
				
				line = line;

				data.push_back(line);

				dataConverted.push_back(List<int>());

				for (int i = 0; i < data.size(); i++)
				{
					/* Comment me please. */
					dataConverted[dataConverted.size() - 1].push_back(std::stoi(data[i]));
				}
			}

			fileStream.close();
		}



		TileMap* tileMap = TileMap::OnCreate(mapName);
		tileMap->SetMapIndexes(dataConverted);
		/* Attach to resource manager. */
		ResourceManager::AttachResource<TileMap>(tileMap);

	}

	void FileSystem::OnReadTileSet(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());
		std::string tileSetName;
		std::string spriteBaseName;
		std::string spriteSheetWidth;
		std::string spriteSheetHeight;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool hasFirstLine = false;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Special Case where it reads the first line. */
				if (!hasFirstLine)
				{
					hasFirstLine = true;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string tileSetProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						tileSetProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (tileSetProperty == "NAME")
							tileSetName = line.substr(0, searchPosition);

						else if (tileSetProperty == "SPRITEBASENAME")
							spriteBaseName = line.substr(0, searchPosition);

						else if (tileSetProperty == "SPRITESHEETWIDTH")
							spriteSheetWidth = line.substr(0, searchPosition);

						else if (tileSetProperty == "SPRITESHEETHEIGHT")
							spriteSheetHeight = line.substr(0, searchPosition);


					}


					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			fileStream.close();
		}

		/* Quick Variables. */
		int width = std::stoi(spriteSheetWidth);
		int height = std::stoi(spriteSheetHeight);
		List<Sprite*> sprites;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				sprites.push_back(ResourceManager::GetResource<Sprite>(spriteBaseName + std::to_string(x) + "-" + std::to_string(y)));
			}
		}

		TileSet* tileSet = TileSet::OnCreate(tileSetName);
		tileSet->SetSprites(sprites);

		/* Attach to resource manager. */
		ResourceManager::AttachResource<TileSet>(tileSet);
	}

	void FileSystem::OnReadSprite(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string spriteType;
		std::string spriteTexture;
		std::string spriteName;
		std::string spriteWidth;
		std::string spriteHeight;
		std::string spriteOffsetX;
		std::string spriteOffsetY;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool hasFirstLine = false;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Special Case where it reads the first line. */
				if (!hasFirstLine)
				{
					spriteType = line;
					hasFirstLine = true;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string spriteProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						spriteProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (spriteProperty == "NAME")
							spriteName = line.substr(0, searchPosition);

						else if (spriteProperty == "TEXTURE")
							spriteTexture = line.substr(0, searchPosition);

						else if (spriteProperty == "WIDTH")
							spriteWidth = line.substr(0, searchPosition);

						else if (spriteProperty == "HEIGHT")
							spriteHeight = line.substr(0, searchPosition);

						else if (spriteProperty == "OFFSETX")
							spriteOffsetX = line.substr(0, searchPosition);

						else if (spriteProperty == "OFFSETY")
							spriteOffsetY = line.substr(0, searchPosition);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			/* Data Values. */
			Texture* texture =	ResourceManager::GetResource<Texture>(spriteTexture);

			if (spriteType != "SPRITE:")
			{
				int width = std::stoi(spriteWidth);
				int height = std::stoi(spriteHeight);

				if (spriteType == "SINGLESPRITE:")
				{
					Vector2 offset = Vector2(std::stoi(spriteOffsetX), std::stoi(spriteOffsetY));
					Sprite::OnCreate(texture, width, height, offset, spriteName);
				}

				if (spriteType == "SPRITESHEET:")
					Sprite::OnCreate(texture, width, height, spriteName);
			}

			else
				Sprite::OnCreate(texture, spriteName);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadFont(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string fontName;
		std::string fontTexture;
		std::string fontWidth;
		std::string fontHeight;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Ignore the first line. */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string fontProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						fontProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (fontProperty == "NAME")
							fontName = line.substr(0, searchPosition);

						else if (fontProperty == "TEXTURE")
							fontTexture = line.substr(0, searchPosition);

						else if (fontProperty == "WIDTH")
							fontWidth = line.substr(0, searchPosition);

						else if (fontProperty == "HEIGHT")
							fontHeight = line.substr(0, searchPosition);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			/* Data Values. */
			Texture* texture = ResourceManager::GetResource<Texture>(fontTexture);

			int width = std::stoi(fontWidth);
			int height = std::stoi(fontHeight);

			Font::OnCreate(texture, width, height, fontName);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadShader(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string shaderName;
		std::string shaderVertex;
		std::string shaderGeometry;
		std::string shaderFragment;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Ignore the first line. */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string shaderProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						shaderProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (shaderProperty == "NAME")
							shaderName = line.substr(0, searchPosition);

						else if (shaderProperty == "VERTEX")
							shaderVertex = line.substr(0, searchPosition);

						else if (shaderProperty == "GEOMETRY")
							shaderGeometry = line.substr(0, searchPosition);

						else if (shaderProperty == "FRAGMENT")
							shaderFragment = line.substr(0, searchPosition);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}
			Shader* shader = nullptr;

			if (shaderGeometry == "*")
				shader = Shader::OnCreate(shaderVertex, shaderFragment, shaderName);
			else
				shader = Shader::OnCreate(shaderVertex, shaderFragment, shaderGeometry, shaderName);

			ResourceManager::AttachResource<Shader>(shader);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadMaterial(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string materialName;
		std::string materialDiffuseSprite;
		std::string materialNormalSprite;
		std::string materialSpecularSprite;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Ignore the first line. */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string materialProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						materialProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (materialProperty == "NAME")
							materialName = line.substr(0, searchPosition);

						else if (materialProperty == "DIFFUSE")
							materialDiffuseSprite = line.substr(0, searchPosition);

						else if (materialProperty == "NORMAL")
							materialNormalSprite = line.substr(0, searchPosition);

						else if (materialProperty == "SPECULAR")
							materialSpecularSprite = line.substr(0, searchPosition);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			Material* material = Material::OnCreate(materialName);

			if (materialDiffuseSprite != "*")
				material->p_Diffuse = ResourceManager::GetResource<Sprite>(materialDiffuseSprite);

			if (materialNormalSprite != "*")
				material->p_Normal = ResourceManager::GetResource<Sprite>(materialNormalSprite);

			if (materialSpecularSprite != "*")
				material->p_Specular = ResourceManager::GetResource<Sprite>(materialSpecularSprite);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadAnimation(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string animationName;
		std::string animationDuration;
		List<std::string> animationKeyFrameData;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Ignore the first line. */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string animationProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						animationProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (animationProperty == "NAME")
							animationName = line.substr(0, searchPosition);

						else if (animationProperty == "DURATION")
							animationDuration = line.substr(0, searchPosition);

						else if(animationProperty == "KEYFRAME")
							animationKeyFrameData.push_back(line);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}

			Animation* animation = Animation::OnCreate(animationName);
			animation->SetDuration(std::stof(animationDuration));

			for (int i = 0; i < animationKeyFrameData.size(); i++)
			{
				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string animationProperty;
				std::string lineSplitter = "->";

				/* Keyframe Data Values. */
				std::string positionX, positionY, rotation, scaleX, scaleY, sprite;

				/* Create the Keyframe. */
				Keyframe* keyframe = KROSS_NEW Keyframe();

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = animationKeyFrameData[i].find(lineSplitter)) != std::string::npos && varSwitch != 6)
				{
					std::string value = animationKeyFrameData[i].substr(0, searchPosition);

					switch (varSwitch)
					{
						case 0:
						{
							if (value != "*")
								positionX = value;

							break;
						}

						case 1:
						{
							if (value != "*")
								positionY = value;

							break;
						}

						case 2:
						{
							if (value != "*")
								rotation = value;

							break;
						}

						case 3:
						{
							if (value != "*")
								scaleX = value;

							break;
						}

						case 4:
						{
							if (value != "*")
								scaleY = value;

							break;
						}

						case 5:
						{
							if (value != "*")
								sprite = value;

							break;
						}
					}

					animationKeyFrameData[i].erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}

				if (positionX != "" && positionY != "")
					keyframe->SetPosition(Vector2(std::stof(positionX), std::stof(positionY)));

				if (rotation != "")
					keyframe->SetRotation(std::stof(rotation));

				if (scaleX != "" && scaleY != "")
					keyframe->SetScale(Vector2(std::stof(scaleX), std::stof(scaleY)));

				if (sprite != "")
					keyframe->SetSprite(ResourceManager::GetResource<Sprite>(sprite));

				/* Add the Keyframe. */
				animation->AttachKeyframe(keyframe);
			}

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}

	void FileSystem::OnReadAudioSource(const std::string& filepath)
	{
		/* Open a Filestream. */
		std::fstream fileStream;
		fileStream.open(filepath.c_str());

		/* Parameter variables. */
		std::string audioSourceName;
		std::string audioSourceFilepath;
		std::string audioSourceStream;

		if (fileStream.is_open())
		{
			/* Variables for opening and reading the file. */
			std::string line;

			bool ignoreFirstLine = true;

			/* Read the file line by line. */
			while (getline(fileStream, line))
			{
				/* Ignore the first line. */
				if (ignoreFirstLine)
				{
					ignoreFirstLine = false;
					continue;
				}

				/* Ignore Comments. */
				if (line.find("//") != std::string::npos)
					continue;

				/* Quick Variables. */
				size_t searchPosition = 0;
				std::string audioSourceProperty;
				std::string lineSplitter = "->";

				int varSwitch = 0;

				/* Keep Searching till we reach the end of the Line.*/
				while ((searchPosition = line.find(lineSplitter)) != std::string::npos && varSwitch != 2)
				{
					/* Grab the Property Type. */
					if (varSwitch == 0)
						audioSourceProperty = line.substr(0, searchPosition);

					/* Grab the Property Value. */
					else
					{
						if (audioSourceProperty == "NAME")
							audioSourceName = line.substr(0, searchPosition);

						else if (audioSourceProperty == "FILEPATH")
							audioSourceFilepath = line.substr(0, searchPosition);

						else if (audioSourceProperty == "STREAM")
							audioSourceStream = line.substr(0, searchPosition);
					}

					line.erase(0, searchPosition + lineSplitter.length());

					/* Up the varaible switch. */
					varSwitch++;
				}
			}
			
			AudioSource* source = AudioSource::OnCreate(audioSourceFilepath, audioSourceName, std::stoi(audioSourceStream));

			ResourceManager::AttachResource<AudioSource>(source);

			fileStream.close();
		}

		else
		{
			fileStream.close();
		}
	}
}