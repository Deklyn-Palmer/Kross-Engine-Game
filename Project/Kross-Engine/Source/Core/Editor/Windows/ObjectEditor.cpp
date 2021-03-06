/*
 *  Author: Chris Deitch.
 *  Editors:
 *      - Chris Deitch.
 *		- Deklyn Palmer.
 *		- Jake Warren.
 */
#include "ObjectEditor.h"

#include "../Editor.h"
#include "../../Debug.h"
#include "../../File-IO/FileSystem.h"
#include "../../Manager/ScriptRegistry.h"


namespace Kross {

	void ObjectEditor::Attach()
	{
		Vector2 viewSize = Editor::GetViewportSize();
		ImVec2 viewsize = ImVec2(viewSize.x * 0.2f, viewSize.y);

		Vector2 viewPos = Editor::GetViewportPosition();
		ImVec2 viewposition = ImVec2(viewPos.x + (viewSize.x - viewsize.x), viewPos.y);

		
		if (p_SelectedObject)
		{
			m_Title = "Object Editor - " + p_SelectedObject->GetName();
		}
		else 
		{
			m_Title = "Object Editor";
		};

		if (p_PreviewPane && p_PreviewPane->IsClosed())
		{
			Editor::DetachEditorWindow(p_PreviewPane);
			p_PreviewPane = nullptr;
		}
		

		ImGui::Begin(m_Title.c_str(), NULL, m_WindowFlags);

		ImGui::SetWindowPos(viewposition);
		ImGui::SetWindowSize(viewsize);
		
		ImGui::BeginTabBar("Object Tab Bar");

		if (ImGui::BeginTabItem("Component Hierarchy"))
		{
			if (p_SelectedObject) 
			{

				/*--------------------------------------------------------------------*/

				if (ImGui::Button("Create as Prefab"))
				{
					
					FileSystem::OnWritePrefab(p_SelectedObject);
				}

				/*--------------------------------------------------------------------*/

				for (int i = 0; i < p_SelectedObject->m_Components.size(); i++)
				{

					ImGui::PushID(i);
					/* Create a Local instance of the Component we are looking at. */
					Component* component = p_SelectedObject->m_Components[i];

					/* For Removing Components, this is more than less the flag for detection. */
					bool isOpen = true;

					/* Transform. (DONE) */
					if (typeid(*component) == typeid(Transform2D))
					{
						Transform2D* transform = (Transform2D*)component;

						if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::PushID("Position");
							ImGui::Columns(2);
							ImGui::SetColumnWidth(0, 70.0f);
							ImGui::Text("Position");
							ImGui::NextColumn();

							ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
							if (ImGui::Button("X"))
							{
								transform->m_Position.x = 0.0f;
							}

							ImGui::SameLine();

							ImGui::DragFloat("##X", &transform->m_Position.x, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							ImGui::PopItemWidth();
							ImGui::SameLine();

							if (ImGui::Button("Y"))
							{
								transform->m_Position.y = 0.0f;
							}

							ImGui::SameLine();

							ImGui::DragFloat("##Y", &transform->m_Position.y, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							ImGui::PopItemWidth();
							ImGui::Columns(1);
							ImGui::PopID();

							ImGui::PushID("Rotation");
							ImGui::Columns(2);
							ImGui::SetColumnWidth(0, 70.0f);
							ImGui::Text("Rotation");
							ImGui::NextColumn();

							ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
							if (ImGui::Button("0"))
							{
								transform->m_Rotation = 0.0f;
							}

							ImGui::SameLine();

							ImGui::DragFloat("##R", &transform->m_Rotation, 0.1f, 0.0f, 360.0f, "%.2fd");
							ImGui::PopItemWidth();
							ImGui::SameLine();

							ImGui::PopItemWidth();
							ImGui::Columns(1);
							ImGui::PopID();

							ImGui::PushID("Scale");
							ImGui::Columns(2);
							ImGui::SetColumnWidth(0, 70.0f);
							ImGui::Text("Scale");
							ImGui::NextColumn();

							ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
							if (ImGui::Button("X"))
							{
								transform->m_Scale.x = 1.0f;
							}

							ImGui::SameLine();

							ImGui::DragFloat("##X", &transform->m_Scale.x, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							ImGui::PopItemWidth();
							ImGui::SameLine();

							if (ImGui::Button("Y"))
							{
								transform->m_Scale.y = 1.0f;
							}

							ImGui::SameLine();

							ImGui::DragFloat("##Y", &transform->m_Scale.y, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							ImGui::PopItemWidth();
							ImGui::Columns(1);
							ImGui::PopID();
						}
					}
					/* Rigidbody. (NEEDS TO BE WORKED ON) */
					else if (typeid(*component) == typeid(Rigidbody2D))
					{
						Rigidbody2D* rb = (Rigidbody2D*)component;
						TileMapRenderer* tm = p_SelectedObject->GetComponent<TileMapRenderer>();

						if (ImGui::CollapsingHeader("Rigidbody", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (tm)
							{
								if (ImGui::Button("Generate Colliders!"))
								{
									rb->DeleteTileMapColliders();
									rb->CreateTileMapColliders(tm->GetTileMap(), tm->GetTileList()[0]);
								}
							}
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<Rigidbody2D>();
						}
					}
					/* Animator. (DONE) */
					else if (typeid(*component) == typeid(Animator))
					{
						Animator* anim = (Animator*)component;
						if (ImGui::CollapsingHeader("Animator", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("Primary Animation: ");
							ImGui::SameLine();
							Animation* animation = anim->GetCurrentAnimation();
							if (ImGui::BeginCombo("##Animation-Choice", (animation) ? animation->GetName().c_str() : "Not-Set", ImGuiComboFlags_NoArrowButton))
							{
								for (int i = 0; i < anim->m_Animations.size(); i++)
								{
									Animation* indexAnimation = anim->m_Animations[i];
									if (ImGui::MenuItem(indexAnimation->GetName().c_str(), "", (indexAnimation == animation)))
									{
										animation = indexAnimation;
									}
								}
								ImGui::EndCombo();
							}
							anim->SetCurrentAnimation((animation) ? animation->GetName() : "NULL");
							ImGui::Separator();
							ImGui::Text("Attached Animations:");
							ImGui::Indent();

							for (int i = 0; i < anim->m_Animations.size(); i++)
							{
								Animation* indexAnimation = anim->m_Animations[i];
								std::string name = indexAnimation->GetName();
								if (ImGui::MenuItem("X"))
								{
									anim->DetachAnimation(name);
								}
								ImGui::SameLine();
								if (ImGui::MenuItem(name.c_str())) {};

							}

							ImGui::Unindent();

							if (ImGui::Button("Attach Animation"))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Animation);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								else if (p_PreviewPane->GetType() != AssetType::Animation)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Animation);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
							}

							if (p_PreviewPane && p_PreviewPane->GetAnimation())
							{
								anim->AttachAnimation(KROSS_NEW Animation(*p_PreviewPane->GetAnimation()));
								p_PreviewPane->SetAnimationDestination(nullptr);
								Editor::DetachEditorWindow(p_PreviewPane);
								p_PreviewPane = nullptr;
							}
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<Animator>();
						}
					}
					/* Audio Player. (NEEDS TO BE WORKED ON) */
					else if (typeid(*component) == typeid(AudioPlayer))
					{
						AudioPlayer* aplayer = (AudioPlayer*)component;


						if (ImGui::CollapsingHeader("AudioPlayer", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* Grab the Properties. */
							AudioProperties* properties = aplayer->GetProperties();

							ImGui::Text("Audio Source: ");
							ImGui::SameLine();
							if (ImGui::Button((aplayer->GetSource() ? aplayer->GetSource()->GetName().c_str() : "Not Set")))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::AudioSource);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								else if (p_PreviewPane->GetType() != AssetType::AudioSource)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::AudioSource);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								

							}

							if (p_PreviewPane && p_PreviewPane->GetAudioSource())
							{
								aplayer->SetAudioSource(KROSS_NEW AudioSource(*p_PreviewPane->GetAudioSource()));
								Editor::DetachEditorWindow(p_PreviewPane);
								p_PreviewPane = nullptr;
							}

							if (ImGui::CollapsingHeader("AudioProperties", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Text("Volume");
								float vol = properties->GetVolume();

								ImGui::SameLine();
								ImGui::DragFloat("##vol", &vol, 0.01f, 0, 1, "%.01fm");
								properties->SetVolume(vol);

								ImGui::Text("PlaySpeed");
								float pspeed = properties->GetPlaySpeed();

								ImGui::SameLine();
								ImGui::DragFloat("##pspeed", &pspeed, 0.01f, 0, 5, "%.01fm");
								properties->SetPlaySpeed(pspeed);


								ImGui::Text("Pan");
								float pan = properties->GetPan();

								ImGui::SameLine();
								ImGui::DragFloat("##pan", &pan, 0.01f, -1, 1, "%.01fm");
								properties->SetPan(pan);


								ImGui::Text("Loop");
								bool loop = properties->GetLoop();

								ImGui::SameLine();
								ImGui::Checkbox("##loop", &loop);

								properties->SetLoop(loop);

							}

						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<AudioPlayer>();
						}
					}
					/* Camera. (DONE) */
					else if (typeid(*component) == typeid(Camera))
					{
						Camera* cam = (Camera*)component;
						if (ImGui::CollapsingHeader("Camera", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							float c_size = cam->GetSize();
							float c_near = cam->GetNear();
							float c_far = cam->GetFar();

							ImGui::Text("Size");
							ImGui::SameLine();
							ImGui::DragFloat("##S", &c_size, 0.1f, 0.5f, 40.0f, "%.2fm");
							ImGui::Text("Near");
							ImGui::SameLine();
							ImGui::DragFloat("##N", &c_near, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							ImGui::Text("Far");
							ImGui::SameLine();
							ImGui::DragFloat("##F", &c_far, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");

							cam->SetSize(c_size);
							cam->SetFar(c_far);
							cam->SetNear(c_near);

							if (ImGui::Button("Reset Default")) {
								cam->SetSize(5.0f);
								cam->SetFar(1.0f);
								cam->SetNear(-1.0f);
							}
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<Camera>();
						}
					}
					/* Particle Emitter. (NEEDS TO BE WORKED ON) */
					else if (typeid(*component) == typeid(ParticleEmitter))
					{
						ParticleEmitter* pEmit = (ParticleEmitter*)component;
						if (ImGui::CollapsingHeader("ParticleEmitter", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<ParticleEmitter>();
						}
					}
					else if (typeid(*component) == typeid(ParticleProperties))
					{
						ParticleProperties* pEmit = (ParticleProperties*)component;
						if (ImGui::CollapsingHeader("ParticleProperties", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{

							float pp_Rad = pEmit->GetRadius();
							int particleCount = pEmit->GetMaxCount();
							uint32 pp_ParticleFlags = pEmit->GetParticleGroupDef()->flags;
							uint16 pp_CatBits = pEmit->GetColliderFilters()->categoryBits;
							uint16 pp_MaskBits = pEmit->GetColliderFilters()->maskBits;

							//Debug::LogLine((int)(pp_ParticleFlags & b2_waterParticle));

							ImGui::Indent();
							if (ImGui::CollapsingHeader("ParticleFlags", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Indent();
								ImGui::PushID("##sprungPartMart");
								if (ImGui::MenuItem("SpringParticle", "", (pp_ParticleFlags & b2_springParticle)))
								{
									pp_ParticleFlags ^= b2_springParticle;
								}
								ImGui::PopID();
								if (ImGui::MenuItem("ElasticParticle", "", (pp_ParticleFlags & b2_elasticParticle)))
								{
									pp_ParticleFlags ^= b2_elasticParticle;
								}
								if (ImGui::MenuItem("ViscousParticle", "", (pp_ParticleFlags & b2_viscousParticle)))
								{
									pp_ParticleFlags ^= b2_viscousParticle;
								}
								if (ImGui::MenuItem("PowderParticle", "", (pp_ParticleFlags & b2_powderParticle)))
								{
									pp_ParticleFlags ^= b2_powderParticle;
								}
								if (ImGui::MenuItem("TensileParticle", "", (pp_ParticleFlags & b2_tensileParticle)))
								{
									pp_ParticleFlags ^= b2_tensileParticle;
								}
								if (ImGui::MenuItem("ColorMixingParticle", "", (pp_ParticleFlags & b2_colorMixingParticle)))
								{
									pp_ParticleFlags ^= b2_colorMixingParticle;
								}
								if (ImGui::MenuItem("BarrierParticle", "", (pp_ParticleFlags & b2_barrierParticle)))
								{
									pp_ParticleFlags ^= b2_barrierParticle;
								}
								if (ImGui::MenuItem("StaticPressureParticle", "", (pp_ParticleFlags & b2_staticPressureParticle)))
								{
									pp_ParticleFlags ^= b2_staticPressureParticle;
								}
								if (ImGui::MenuItem("ReactiveParticle", "", (pp_ParticleFlags & b2_reactiveParticle)))
								{
									pp_ParticleFlags ^= b2_reactiveParticle;
								}
								if (ImGui::MenuItem("RepulsiveParticle", "", (pp_ParticleFlags & b2_repulsiveParticle)))
								{
									pp_ParticleFlags ^= b2_repulsiveParticle;
								}
								ImGui::Unindent();
							}

							if (ImGui::CollapsingHeader("Collider Mask", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Indent();
								if (ImGui::MenuItem("BackGround", "", (pp_CatBits & (uint16)ColliderFilters::BackGround)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::BackGround;
								}
								if (ImGui::MenuItem("Environment", "", (pp_CatBits & (uint16)ColliderFilters::Environment)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Environment;
								}
								if (ImGui::MenuItem("Level", "", (pp_CatBits & (uint16)ColliderFilters::Level)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Level;
								}
								if (ImGui::MenuItem("Fluid", "", (pp_CatBits & (uint16)ColliderFilters::Fluid)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Fluid;
								}
								if (ImGui::MenuItem("Light", "", (pp_CatBits & (uint16)ColliderFilters::Light)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Light;
								}
								if (ImGui::MenuItem("Player", "", (pp_CatBits & (uint16)ColliderFilters::Player)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Player;
								}
								if (ImGui::MenuItem("Enemy", "", (pp_CatBits & (uint16)ColliderFilters::Enemy)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Enemy;
								}
								if (ImGui::MenuItem("Weapon", "", (pp_CatBits & (uint16)ColliderFilters::Weapon)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Weapon;
								}
								if (ImGui::MenuItem("Chain", "", (pp_CatBits & (uint16)ColliderFilters::Chain)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Chain;
								}
								if (ImGui::MenuItem("Puzzle", "", (pp_CatBits & (uint16)ColliderFilters::Puzzle)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Puzzle;
								}
								if (ImGui::MenuItem("Empty", "", (pp_CatBits & (uint16)ColliderFilters::Empty)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Empty;
								}
								ImGui::Unindent();
							}

							if (ImGui::CollapsingHeader("Collides With", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Indent();
								if (ImGui::MenuItem("BackGround", "", (pp_MaskBits & (uint16)ColliderFilters::BackGround)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::BackGround;
								}
								if (ImGui::MenuItem("Environment", "", (pp_MaskBits & (uint16)ColliderFilters::Environment)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Environment;
								}
								if (ImGui::MenuItem("Level", "", (pp_MaskBits & (uint16)ColliderFilters::Level)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Level;
								}
								if (ImGui::MenuItem("Fluid", "", (pp_MaskBits & (uint16)ColliderFilters::Fluid)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Fluid;
								}
								if (ImGui::MenuItem("Light", "", (pp_MaskBits & (uint16)ColliderFilters::Light)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Light;
								}
								if (ImGui::MenuItem("Player", "", (pp_MaskBits & (uint16)ColliderFilters::Player)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Player;
								}
								if (ImGui::MenuItem("Enemy", "", (pp_MaskBits & (uint16)ColliderFilters::Enemy)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Enemy;
								}
								if (ImGui::MenuItem("Weapon", "", (pp_MaskBits & (uint16)ColliderFilters::Weapon)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Weapon;
								}
								if (ImGui::MenuItem("Chain", "", (pp_MaskBits & (uint16)ColliderFilters::Chain)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Chain;
								}
								if (ImGui::MenuItem("Puzzle", "", (pp_MaskBits & (uint16)ColliderFilters::Puzzle)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Puzzle;
								}
								if (ImGui::MenuItem("Empty", "", (pp_MaskBits & (uint16)ColliderFilters::Empty)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Empty;
								}
								ImGui::Unindent();
							}
							ImGui::Unindent();

							ImGui::Text("Particle Radius: ");
							ImGui::SameLine();
							ImGui::DragFloat("##Rad", &pp_Rad, 0.005f, 0.005f, 1.0f, "%.3fm");

							ImGui::Text("Particle Count: ");
							ImGui::SameLine();
							ImGui::DragInt("##Count", &particleCount, 2, 0, INT_MAX, "%d");

							pEmit->SetGroupFlags(b2_rigidParticleGroup);
							pEmit->SetRadius(pp_Rad);
							pEmit->SetParticleFlags(pp_ParticleFlags);
							pEmit->SetColliderFilters(pp_CatBits, pp_MaskBits);
							pEmit->SetMaxCount(particleCount);

						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<ParticleProperties>();
						}
					}

					/* COG */
					else if (typeid(*component) == typeid(Cog)) 
					{
						if (ImGui::CollapsingHeader("Cog", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							Cog* cog = (Cog*)component;
							float cog_speed = cog->GetMotorSpeed();
							float cog_torque = cog->GetMaxMotorTorque();

							ImGui::Text("Cog Speed:");
							ImGui::DragFloat("##cogSpeed", &cog_speed, 0.005f, 0.005f, 1.0f, "%.3fm");


							ImGui::Text("Cog Max Torque:");
							ImGui::DragFloat("##cogTorque", &cog_torque, 0.005f, 0.005f, 1.0f, "%.3fm");

							char startNameBuffer[128] = { '/0' };
							std::string startReserveNameString = cog->GetStartReserveName();
							for (int i = 0; i < startReserveNameString.length(); i++)
							{
								startNameBuffer[i] = startReserveNameString[i];
							}

							ImGui::Text("Connected Start Object:");
							ImGui::InputText("##StartReserveCog", &startNameBuffer[0], 128, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);


							cog->UpdateMotorSpeed(cog_speed);
							cog->UpdateMaxMotorTorque(cog_torque);
							cog->SetStartReserveName(startNameBuffer);
						}
					}

					/* RopeAvatar. (Work To Do) */
					else if (typeid(*component) == typeid(RopeAvatar))
					{
						if (ImGui::CollapsingHeader("Rope Avatar", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{

							RopeAvatar* ropeAvatar = (RopeAvatar*)component;
							float rp_ChainLength = ropeAvatar->GetChainLinkLength();
							bool rp_isBreakable = ropeAvatar->IsBreakable();
							bool rp_startIsStatic = ropeAvatar->IsStartStatic();

							float rp_startAttachX = ropeAvatar->GetAttachmentPointOnStartObj().x;
							float rp_startAttachY = ropeAvatar->GetAttachmentPointOnStartObj().y;
							float rp_endAttachX = ropeAvatar->GetAttachmentPointOnEndObj().x;
							float rp_endAttachY = ropeAvatar->GetAttachmentPointOnEndObj().y;


							char startNameBuffer[128] = {'/0'};
							std::string startReserveNameString = ropeAvatar->GetStartReserveName();
							for (int i = 0; i < startReserveNameString.length(); i++)
							{
								startNameBuffer[i] = startReserveNameString[i];
							}


					



							char endNameBuffer[128] = { '/0' };
							std::string endReserveNameString = ropeAvatar->GetEndReserveName();
							for (int i = 0; i < endReserveNameString.length(); i++)
							{
								endNameBuffer[i] = endReserveNameString[i];
							}



							std::vector<Vector2> rp_positions = ropeAvatar->GetBasePositions();



							ImGui::Indent();
							if (ImGui::CollapsingHeader("Positions", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								for (int i = 0; i < rp_positions.size(); i++) 
								{
									float rp_seg_posX = rp_positions[i].x;
									float rp_seg_posY = rp_positions[i].y;
									std::string positionIdentity = ("Postition: " + std::to_string(i+1));
									ImGui::Text(positionIdentity.c_str());
									ImGui::Text("X-Pos");
									ImGui::SameLine();
									std::string positionIdentityX = ("##rpPosX" + std::to_string(i));
									ImGui::DragFloat(positionIdentityX.c_str(), & rp_seg_posX, 0.1f, -FLT_MAX, FLT_MAX);

									ImGui::Text("Y-Pos");
									ImGui::SameLine();
									std::string positionIdentityY = ("##rpPosY" + std::to_string(i));
									ImGui::DragFloat(positionIdentityY.c_str(), &rp_seg_posY, 0.1f, -FLT_MAX, FLT_MAX);

									rp_positions[i].x = rp_seg_posX;
									rp_positions[i].y = rp_seg_posY;
									Vector2 newPos = Vector2(rp_seg_posX, rp_seg_posY);
									ropeAvatar->SetBasePosition(i, newPos);

								}
								if (ImGui::Button("AddPosition")) 
								{
									ropeAvatar->AttachPosition(Vector2(0.0f, 0.0f));
								}
							}
							ImGui::Unindent();

							ImGui::Text("Chain Segment Length: ");
							ImGui::DragFloat("##chainLength", &rp_ChainLength, 0.005f, 0.005f, 1.0f, "%.3fm");

							ImGui::Text("Is Breakable: ");
							ImGui::SameLine();
							ImGui::Checkbox("##isBreak", &rp_isBreakable);
							
							ImGui::Text("Start is Static: ");
							ImGui::SameLine();
							ImGui::Checkbox("##isStartStatic", &rp_startIsStatic);

							ImGui::Text("Connected Start Object:");
							ImGui::InputText("##StartReserve", &startNameBuffer[0], 128, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);


							ImGui::Text("StartAttachPosition:");
							ImGui::Text("X:");
							ImGui::SameLine();
							ImGui::DragFloat("##rp_StartPosX", &rp_startAttachX, 0.1f, -FLT_MAX, FLT_MAX);
							
							ImGui::Text("Y:");
							ImGui::SameLine();
							ImGui::DragFloat("##rp_StartPosY", &rp_startAttachY, 0.1f, -FLT_MAX, FLT_MAX);


							ImGui::Text("Connected End Object:");
							ImGui::InputText("##EndReserve", &endNameBuffer[0], 128, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);
							ImGui::Text("EndAttachPosition:");
							ImGui::Text("X:");
							ImGui::SameLine();
							ImGui::DragFloat("##rp_EndPosX", &rp_endAttachX, 0.1f, -FLT_MAX, FLT_MAX);
							
							ImGui::Text("Y:");
							ImGui::SameLine();
							ImGui::DragFloat("##rp_EndPosY", &rp_endAttachY, 0.1f, -FLT_MAX, FLT_MAX);


							ropeAvatar->SetStartReserveName(startNameBuffer);
							ropeAvatar->SetAttachmentPointOnStartObj(glm::vec2(rp_startAttachX,rp_startAttachY));
							ropeAvatar->SetEndReserveName(endNameBuffer);
							ropeAvatar->SetAttachmentPointOnEndObj(glm::vec2(rp_endAttachX, rp_endAttachY));
							ropeAvatar->SetChainLinkLength(rp_ChainLength);
							ropeAvatar->SetStartStatic(rp_startIsStatic);
							ropeAvatar->SetBreakable(rp_isBreakable);
						}
					}
					/* Sprite Renderer. (DONE) */
					else if (typeid(*component) == typeid(SpriteRenderer))
					{
						SpriteRenderer* rend = (SpriteRenderer*)component;
						if (ImGui::CollapsingHeader("SpriteRenderer", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							Vector4 col = rend->GetColour();
							float s_col[4] = { col.r, col.g, col.b, col.a };
							Material* s_mat = rend->GetMaterial();
							bool s_fx = rend->GetFlipX();
							bool s_fy = rend->GetFlipY();
							int s_depth = rend->GetDepth();


							if (ImGui::Button((s_mat) ? s_mat->GetName().c_str() : "Not-Selected"))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Material);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}

								else if (p_PreviewPane->GetType() != AssetType::Material)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Material);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								p_PreviewPane->SetMaterialDestination(s_mat);
							}

							ImGui::Indent();
							ImGui::Indent();
							if (ImGui::CollapsingHeader(((rend->GetMaterial()) ? (std::string)(rend->GetMaterial()->GetName() + " - Material").c_str() : "Material").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
							{
								char m_cha[1024]{ '/0' };
								std::string m_text = "NULL";
								if (s_mat)
								{
									m_text = rend->GetMaterial()->GetName();
								}
								strncpy_s(m_cha, m_text.c_str(), 1024);

								float width = ImGui::GetContentRegionAvail().x;
								ImGui::Text("Name:");
								ImGui::InputText("##T", &m_cha[0], 1024);
								std::string toSet = m_cha;
								if (s_mat)
								{
									rend->GetMaterial()->SetName(toSet);
								}

								ImGui::Text("Diffuse");
								ImGui::SameLine();
								if (ImGui::Button((s_mat) ? s_mat->m_Diffuse->GetName().c_str() : "NULL"))
								{
									if (s_mat)
									{
										if (!p_PreviewPane)
										{
											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}

										else if (p_PreviewPane->GetType() != AssetType::Sprite)
										{
											Editor::DetachEditorWindow(p_PreviewPane);

											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}
										p_PreviewPane->SetSpriteDestination(s_mat->m_Diffuse);
									}
								}

								ImGui::Text("Normal");
								ImGui::SameLine();
								if (ImGui::Button((s_mat) ? s_mat->m_Normal->GetName().c_str() : "NULL"))
								{
									if (s_mat)
									{
										if (!p_PreviewPane)
										{
											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}

										else if (p_PreviewPane->GetType() != AssetType::Sprite)
										{
											Editor::DetachEditorWindow(p_PreviewPane);

											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}
										p_PreviewPane->SetSpriteDestination(s_mat->m_Normal);
									}
								}

								ImGui::Text("Specular");
								ImGui::SameLine();
								if (ImGui::Button((s_mat) ? s_mat->m_Specular->GetName().c_str() : "NULL"))
								{
									if (s_mat)
									{
										if (!p_PreviewPane)
										{
											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}
										else if (p_PreviewPane->GetType() != AssetType::Sprite)
										{
											Editor::DetachEditorWindow(p_PreviewPane);

											p_PreviewPane = KROSS_NEW AssetPreview();
											p_PreviewPane->SetType(AssetType::Sprite);
											p_PreviewPane->SetDimensions();
											p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
											Editor::AttachEditorWindow(p_PreviewPane);
										}
										p_PreviewPane->SetSpriteDestination(s_mat->m_Specular);
									}
								}
							}
							ImGui::Unindent();
							ImGui::Unindent();

							ImGui::Separator();
							ImGui::Text("Colour");
							ImGui::ColorPicker4("##C", s_col);
							col = Vector4{ s_col[0],s_col[1], s_col[2], s_col[3] };

							ImGui::Text("");

							ImGui::Text("Flip X/Y");
							if (ImGui::Button("X")) {
								s_fx = (!rend->GetFlipX());
							}
							ImGui::SameLine();
							if (ImGui::Button("Y"))
							{
								s_fy = (!rend->GetFlipY());
							}

							ImGui::Text("Depth");
							ImGui::SameLine();
							ImGui::DragInt("##s_deppy", &s_depth, 1, 0, INTMAX_MAX);


							if (p_PreviewPane && p_PreviewPane->GetSprite())
							{
								if (s_mat->m_Diffuse == p_PreviewPane->GetSpriteDestination())
								{
									s_mat->m_Diffuse = p_PreviewPane->GetSprite();
									p_PreviewPane->SetSpriteDestination(s_mat->m_Diffuse);
								}

								else if (s_mat->m_Normal == p_PreviewPane->GetSpriteDestination())
								{
									s_mat->m_Normal = p_PreviewPane->GetSprite();
									p_PreviewPane->SetSpriteDestination(s_mat->m_Normal);
								}

								else if (s_mat->m_Specular == p_PreviewPane->GetSpriteDestination())
								{
									s_mat->m_Specular = p_PreviewPane->GetSprite();
									p_PreviewPane->SetSpriteDestination(s_mat->m_Specular);
								}
							}

							if (p_PreviewPane && p_PreviewPane->GetMaterial())
							{
								if (s_mat == p_PreviewPane->GetMaterialDestination())
								{
									s_mat = p_PreviewPane->GetMaterial();
									p_PreviewPane->SetMaterialDestination(s_mat);
								}
							}

							rend->SetColour(col);
							rend->SetFlipX(s_fx);
							rend->SetFlipY(s_fy);
							rend->SetMaterial(s_mat);
							rend->SetDepth(s_depth);

							ImGui::Separator();
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<SpriteRenderer>();
						}
					}
					/* Text Renderer. (DONE) */
					else if (typeid(*component) == typeid(TextRenderer))
					{
						TextRenderer* rend = (TextRenderer*)component;

						if (ImGui::CollapsingHeader("Text Renderer", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							Font* t_font = rend->GetFont();
							float t_size = rend->GetTextSize();
							std::string t_text = rend->GetText();
							Vector4 col = rend->GetColour();
							float t_col[4] = { col.r, col.g, col.b, col.a };

							char t_cha[1024]{ '/0' };
							strncpy_s(t_cha, t_text.c_str(), 1024);


							ImGui::Text("Text");
							ImGui::InputText("##T", &t_cha[0], 1024);


							ImGui::Text("Font");
							ImGui::SameLine();
							if (ImGui::Button((t_font) ? t_font->GetName().c_str() : "Not-Selected"))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Font);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}

								else if (p_PreviewPane->GetType() != AssetType::Font)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::Font);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								p_PreviewPane->SetFontDestination(t_font);
							}

							ImGui::Text("Colour");
							ImGui::ColorPicker4("##C", t_col);

							ImGui::Text("Size");
							ImGui::SameLine();
							ImGui::DragFloat("##S", &t_size, 0.1f, 0.5f, 40.0f, "%.2fm");


							col = Vector4{ t_col[0],t_col[1], t_col[2], t_col[3] };


							std::string toSet = t_cha;

							if (p_PreviewPane && p_PreviewPane->GetFont())
							{
								if (t_font == p_PreviewPane->GetFontDestination())
								{
									t_font = p_PreviewPane->GetFont();
									p_PreviewPane->SetFontDestination(t_font);
								}
							}

							rend->SetText(toSet);
							rend->SetColour(col);
							rend->SetFont(t_font);
							rend->SetTextSize(t_size);
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<TextRenderer>();
						}
					}
					/* Collider. (DONE) */
					else if (typeid(*component) == typeid(Collider))
					{
						Collider* col = (Collider*)component;
						if (ImGui::CollapsingHeader("Collider", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							ShapeType c_Type = col->GetShapeType();
							float c_Width = col->GetWidth();
							float c_Height = col->GetHeight();
							float c_Radius = col->GetRadius();
							float c_Frict = col->GetFriction();
							float c_Mass = col->GetDensity();
							bool c_IsStatic = col->IsStatic();
							bool c_FixedRot = col->IsRotationLocked();
							bool c_IsTileMap = col->IsTileMapCollider();
							uint16 pp_CatBits = col->GetCollisionFilters()->categoryBits;
							uint16 pp_MaskBits = col->GetCollisionFilters()->maskBits;

							if (ImGui::BeginCombo("##Shape", ((std::string)((std::string)((c_Type == ShapeType::Box) ? "Box" : ((c_Type == ShapeType::Circle) ? "Circle" : "Capsule")))).c_str(), ImGuiComboFlags_NoArrowButton))
							{
								if (ImGui::MenuItem("Box"))
								{
									col->SetShapeType(ShapeType::Box);
								}
								if (ImGui::MenuItem("Circle"))
								{
									col->SetShapeType(ShapeType::Circle);
								}
								if (ImGui::MenuItem("Capsule"))
								{
									col->SetShapeType(ShapeType::Capsule);
								}
								ImGui::EndCombo();
							}
							ImGui::Separator();

							ImGui::Text("Width");
							ImGui::SameLine();
							if (c_Type == ShapeType::Box)
							{
								ImGui::DragFloat("##c_Width", &c_Width, 0.1f, 0.0f, 100.0f, "%.2fm");
							}
							else
							{
								char buffer[512];
								ImGui::InputTextEx("##c_Width", std::to_string(c_Width).c_str(), &buffer[0], 512, ImVec2(0.0f, 0.0f), ImGuiInputTextFlags_ReadOnly);
							}

							ImGui::Text("Height");
							ImGui::SameLine();
							if (c_Type == ShapeType::Box || c_Type == ShapeType::Capsule)
							{
								ImGui::DragFloat("##c_Height", &c_Height, 0.1f, 0.0f, 100.0f, "%.2fm");
							}
							else
							{
								char buffer[512];
								ImGui::InputTextEx("##c_Height", std::to_string(c_Height).c_str(), &buffer[0], 512, ImVec2(0.0f, 0.0f), ImGuiInputTextFlags_ReadOnly);
							}

							ImGui::Text("Radius");
							ImGui::SameLine();
							if (c_Type == ShapeType::Circle || c_Type == ShapeType::Capsule)
							{
								ImGui::PushTextWrapPos(-10.0f);
								ImGui::DragFloat("##c_Rad", &c_Radius, 0.1f, 0.0f, 50.0f, "%.2fm");
								ImGui::PopTextWrapPos();
							}
							else
							{
								char buffer[512];
								ImGui::InputTextEx("##c_Rad", std::to_string(c_Radius).c_str(), &buffer[0], 512, ImVec2(0.0f, 0.0f), ImGuiInputTextFlags_ReadOnly);
							}


							if (ImGui::CollapsingHeader("Collider Mask", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Indent();
								if (ImGui::MenuItem("BackGround", "", (pp_CatBits & (uint16)ColliderFilters::BackGround)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::BackGround;
								}
								if (ImGui::MenuItem("Environment", "", (pp_CatBits & (uint16)ColliderFilters::Environment)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Environment;
								}
								if (ImGui::MenuItem("Level", "", (pp_CatBits & (uint16)ColliderFilters::Level)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Level;
								}
								if (ImGui::MenuItem("Fluid", "", (pp_CatBits & (uint16)ColliderFilters::Fluid)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Fluid;
								}
								if (ImGui::MenuItem("Light", "", (pp_CatBits & (uint16)ColliderFilters::Light)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Light;
								}
								if (ImGui::MenuItem("Player", "", (pp_CatBits & (uint16)ColliderFilters::Player)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Player;
								}
								if (ImGui::MenuItem("Enemy", "", (pp_CatBits & (uint16)ColliderFilters::Enemy)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Enemy;
								}
								if (ImGui::MenuItem("Weapon", "", (pp_CatBits & (uint16)ColliderFilters::Weapon)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Weapon;
								}
								if (ImGui::MenuItem("Chain", "", (pp_CatBits & (uint16)ColliderFilters::Chain)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Chain;
								}
								if (ImGui::MenuItem("Puzzle", "", (pp_CatBits & (uint16)ColliderFilters::Puzzle)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Puzzle;
								}
								if (ImGui::MenuItem("Empty", "", (pp_CatBits & (uint16)ColliderFilters::Empty)))
								{
									pp_CatBits ^= (uint16)ColliderFilters::Empty;
								}
								ImGui::Unindent();
							}

							if (ImGui::CollapsingHeader("Collides With", (bool*)false, ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Indent();
								if (ImGui::MenuItem("MaskBackGround", "", (pp_MaskBits & (uint16)ColliderFilters::BackGround)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::BackGround;
								}
								if (ImGui::MenuItem("MaskEnvironment", "", (pp_MaskBits & (uint16)ColliderFilters::Environment)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Environment;
								}
								if (ImGui::MenuItem("MaskLevel", "", (pp_MaskBits & (uint16)ColliderFilters::Level)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Level;
								}
								if (ImGui::MenuItem("MaskFluid", "", (pp_MaskBits & (uint16)ColliderFilters::Fluid)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Fluid;
								}
								if (ImGui::MenuItem("MaskLight", "", (pp_MaskBits & (uint16)ColliderFilters::Light)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Light;
								}
								if (ImGui::MenuItem("MaskPlayer", "", (pp_MaskBits & (uint16)ColliderFilters::Player)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Player;
								}
								if (ImGui::MenuItem("MaskEnemy", "", (pp_MaskBits & (uint16)ColliderFilters::Enemy)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Enemy;
								}
								if (ImGui::MenuItem("MaskWeapon", "", (pp_MaskBits & (uint16)ColliderFilters::Weapon)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Weapon;
								}
								if (ImGui::MenuItem("MaskChain", "", (pp_MaskBits & (uint16)ColliderFilters::Chain)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Chain;
								}
								if (ImGui::MenuItem("MaskPuzzle", "", (pp_MaskBits & (uint16)ColliderFilters::Puzzle)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Puzzle;
								}
								if (ImGui::MenuItem("MaskEmpty", "", (pp_MaskBits & (uint16)ColliderFilters::Empty)))
								{
									pp_MaskBits ^= (uint16)ColliderFilters::Empty;
								}
								ImGui::Unindent();
							}

							ImGui::Text("Friction");
							ImGui::SameLine();
							ImGui::DragFloat("##Frict", &c_Frict, 0.1f, 0.0f, 1.0f, "%.2fm");

							ImGui::Text("Mass");
							ImGui::SameLine();
							ImGui::DragFloat("##Mass", &c_Mass, 0.1f, 0.0f, 100.0f, "%.2fm");

							ImGui::Text("Static?");
							ImGui::SameLine();
							ImGui::Checkbox("##S", &c_IsStatic);

							ImGui::Text("Fixed Rotation?");
							ImGui::SameLine();
							ImGui::Checkbox("##FR", &c_FixedRot);

							ImGui::Text("Tilemap?");
							ImGui::SameLine();
							ImGui::Checkbox("##TI", &c_IsTileMap);

							if (c_Type == ShapeType::Box)
							{
								col->SetWidth(c_Width);
							}
							col->SetHeight(c_Height);

							if (c_Type == ShapeType::Circle || c_Type == ShapeType::Capsule)
							{
								col->SetRadius(c_Radius);
							}

							col->SetFriction(c_Frict);
							col->SetStatic(c_IsStatic);
							col->SetRotationLock(c_FixedRot);
							col->SetTileMapCollider(c_IsTileMap);
							col->SetDensity(c_Mass);
							b2Filter* tempFilter = KROSS_NEW b2Filter();
							tempFilter->categoryBits = pp_CatBits;
							tempFilter->maskBits = pp_MaskBits;
							col->SetCollisonFilters(tempFilter);

							delete tempFilter;
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<Collider>();
						}
					}
					/* Tile Map Renderer. (DONE) */
					else if (typeid(*component) == typeid(TileMapRenderer))
					{
						TileMapRenderer* rend = (TileMapRenderer*)component;
						if (ImGui::CollapsingHeader("Tile Map Renderer", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("Tile Set: ");
							ImGui::SameLine();
							if (ImGui::Button((rend->GetTileSet()) ? rend->GetTileSet()->GetName().c_str() : "Not-Set"))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::TileSet);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								else if (p_PreviewPane->GetType() != AssetType::TileSet)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::TileSet);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
							}

							ImGui::Separator();

							ImGui::Text("Tile Map: ");
							ImGui::SameLine();
							if (ImGui::Button((rend->GetTileMap()) ? rend->GetTileMap()->GetName().c_str() : "Not-Set"))
							{
								if (!p_PreviewPane)
								{
									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::TileMap);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
								else if (p_PreviewPane->GetType() != AssetType::TileMap)
								{
									Editor::DetachEditorWindow(p_PreviewPane);

									p_PreviewPane = KROSS_NEW AssetPreview();
									p_PreviewPane->SetType(AssetType::TileMap);
									p_PreviewPane->SetDimensions();
									p_PreviewPane->SetPosition((viewPos.x + (viewSize.x / 2.0f)) - (256.0f / 2.0f), (viewPos.y + (viewSize.y / 2.0f)) - (384.0f / 2.0f));
									Editor::AttachEditorWindow(p_PreviewPane);
								}
							}

							if (p_PreviewPane)
							{
								if (p_PreviewPane->GetTileSet())
								{
									rend->SetTileSet(p_PreviewPane->GetTileSet());
									Editor::DetachEditorWindow(p_PreviewPane);
									p_PreviewPane = nullptr;
								}
								else if (p_PreviewPane->GetTileMap())
								{
									rend->SetTileMap(p_PreviewPane->GetTileMap());
									Editor::DetachEditorWindow(p_PreviewPane);
									p_PreviewPane = nullptr;
								}
							}
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<TileMapRenderer>();
						}
					}
					else if (typeid(*component) == typeid(PlayerController))
					{
						PlayerController* controller = (PlayerController*)component;
						if (ImGui::CollapsingHeader("Player Controller", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							float groundSpeed = controller->GetGroundSpeed();
							float maxGroundSpeed = controller->GetMaxGroundSpeed();

							float airSpeed = controller->GetAirSpeed();
							float maxAirSpeed = controller->GetMaxAirSpeed();

							float swimSpeed = controller->GetSwimSpeed();
							float maxSwimSpeed = controller->GetMaxSwimSpeed();

							float jumpStrength = controller->GetJumpStrength();
							int maxJumps = controller->GetMaxJumpCount();

							ImGui::Text("Speeds:");
							ImGui::Separator();
							ImGui::Indent();
							ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

							ImGui::Text("Ground Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerGroundSpeed", &groundSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetGroundSpeed(groundSpeed);

							ImGui::Text("Max Ground Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerMaxGroundSpeed", &maxGroundSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetMaxGroundSpeed(maxGroundSpeed);


							ImGui::Text("Air Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerAirSpeed", &airSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetAirSpeed(airSpeed);

							ImGui::Text("Max Air Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerMaxAirSpeed", &maxAirSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetMaxAirSpeed(maxAirSpeed);


							ImGui::Text("Swim Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerSwimSpeed", &swimSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetAirSpeed(swimSpeed);

							ImGui::Text("Max Swim Speed: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerMaxSwimSpeed", &maxSwimSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetMaxSwimSpeed(maxSwimSpeed);
							ImGui::Unindent();

							ImGui::Text("Jumping:");
							ImGui::Separator();

							ImGui::Indent();
							ImGui::Text("Max Jump Count: ");
							ImGui::SameLine();
							ImGui::DragInt("##PlayerMaxJumps", &maxJumps, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetMaxJumpCount(maxJumps);

							ImGui::Text("Jump Strength: ");
							ImGui::SameLine();
							ImGui::DragFloat("##PlayerJumpHeight", &jumpStrength, 0.1f, -FLT_MAX, FLT_MAX, "%.2fm");
							controller->SetJumpStrength(jumpStrength);

							ImGui::Unindent();
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<PlayerController>();
						}
					}

					else if (typeid(*component) == typeid(UITransform))
					{
						UITransform* transform = (UITransform*)component;
						if (ImGui::CollapsingHeader("UI Transform", &isOpen, ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* Gather Data. */
							Vector2 actualRatio = transform->GetRatio();
							float ratio[2] = { actualRatio.x, actualRatio.y };

							ImGui::Text("NOTE: 0,0 is Top Left. 1,1 is Bottom Right.");

							ImGui::Text("Ratio: ");
							ImGui::SameLine();
							ImGui::DragFloat2("##ratio", &ratio[0], 0.01f, 0.0f, 1.0f, "%.3f");

							transform->SetRatio(Vector2(ratio[0], ratio[1]));
						}

						if (!isOpen)
						{
							p_SelectedObject->DetachComponent<UITransform>();
						}
					}
					else
					{
						ImGui::CollapsingHeader(((Script*)component)->GetName().c_str(), &isOpen, ImGuiTreeNodeFlags_Leaf);

						if (!isOpen)
						{
							delete p_SelectedObject->m_Components[i];
							p_SelectedObject->m_Components.erase(p_SelectedObject->m_Components.begin() + i);
						}
					}

					ImGui::PopID();

					ImGui::Separator();
				}

				if (ImGui::BeginCombo("##AddComp", "Add Component", ImGuiComboFlags_NoArrowButton))
				{
					if (ImGui::MenuItem("Animator"))
					{
						p_SelectedObject->AttachComponent<Animator>();
					}

					if (ImGui::MenuItem("Audio Player"))
					{
						p_SelectedObject->AttachComponent<AudioPlayer>();
					}

					if (ImGui::MenuItem("Camera"))
					{
						p_SelectedObject->AttachComponent<Camera>();
					}

					if (ImGui::BeginMenu("UI"))
					{
						if (ImGui::MenuItem("UI Transform"))
						{
							p_SelectedObject->AttachComponent<UITransform>();
						}
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Physics"))
					{
						if (ImGui::MenuItem("Collider"))
						{
							p_SelectedObject->AttachComponent<Collider>();
						}
						if (ImGui::MenuItem("Rigidbody2D"))
						{
							p_SelectedObject->AttachComponent<Rigidbody2D>();
						}
						if (ImGui::MenuItem("Player Controller"))
						{
							p_SelectedObject->AttachComponent<PlayerController>();
						}
						if (ImGui::MenuItem("Particle Emitter"))
						{
							p_SelectedObject->AttachComponent<ParticleEmitter>();
						}
						if (ImGui::MenuItem("Particle Properties"))
						{
							p_SelectedObject->AttachComponent<ParticleProperties>();
						}
						if (ImGui::MenuItem("Rope Avatar"))
						{
							p_SelectedObject->AttachComponent<RopeAvatar>();
						}
						
						//if (ImGui::MenuItem("Rope Properties"))
						//	p_SelectedObject->AttachComponent<RopeProperties>();

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Scripts"))
					{
						for (int i = 0; i < ScriptRegistry::m_Instance->m_Scripts.size(); i++)
						{
							Script* script = ScriptRegistry::m_Instance->m_Scripts[i];
							if (ImGui::MenuItem(script->GetName().c_str()))
							{
								Script* addScript = ScriptRegistry::GetScript(i);
								addScript->m_GameObject = p_SelectedObject;
								p_SelectedObject->m_Components.push_back(addScript);
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Rendering"))
					{
						if (ImGui::MenuItem("Sprite Renderer"))
						{
							p_SelectedObject->AttachComponent<SpriteRenderer>();
						}
						if (ImGui::MenuItem("Text Renderer"))
						{
							p_SelectedObject->AttachComponent<TextRenderer>();
						}
						if (ImGui::MenuItem("Tile Map Renderer"))
						{
							p_SelectedObject->AttachComponent<TileMapRenderer>();
						}
						ImGui::EndMenu();
					}
				}
			}
			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Object Properties"))
		{
			if (p_SelectedObject)
			{
				ImGui::Text("Name: ");
				ImGui::SameLine();
				char name[64]{ '\0' };
				for (int i = 0; i < p_SelectedObject->GetName().size(); i++)
				{
					name[i] = p_SelectedObject->GetName()[i];
				}
				if (ImGui::InputTextEx("##Obejct-Name", p_SelectedObject->GetName().c_str(), &name[0], sizeof(name), ImVec2(0.0f, 0.0f), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					p_SelectedObject->SetName(name);
				}
				ImGui::Spacing();

				bool enable = p_SelectedObject->Enabled();
				ImGui::Text("Enable: ");
				ImGui::SameLine();
				ImGui::Checkbox("##EnableObject", &enable);

				p_SelectedObject->SetEnable(enable);

				ImGui::Spacing();
				
				bool isStatic = p_SelectedObject->IsStatic();
				ImGui::Text("Static: ");
				ImGui::SameLine();
				ImGui::Checkbox("##StaticObject", &isStatic);

				p_SelectedObject->SetStatic(isStatic);

				ImGui::Spacing();

				ImGui::Text("Layer:");
				LayerName names = LayerName();
				if (ImGui::BeginCombo("##Layer", names[(int)p_SelectedObject->GetLayer()].c_str(), ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < (int)Layer::Count; i++)
					{
						if (ImGui::MenuItem(names[i].c_str()))
						{
							p_SelectedObject->SetLayer((Layer)i);
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::EndTabItem();
		}

		/* Gizmo Drawing. */
		if (p_SelectedObject)
		{
			
		}

		ImGui::EndTabBar();

		ImGui::End();
		
	}

	void ObjectEditor::SetFlags()
	{
		m_WindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	}

	void ObjectEditor::OnStart()
	{
		SetFlags();
		m_Title = "Object Editor."; //placeholder title for safety.

	}
}