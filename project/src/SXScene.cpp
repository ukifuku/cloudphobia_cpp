#include "SXScene.h"

TSceneManager::TSceneManager( TDGCarad* owner )
{
	FOwner = owner;
	FScaleCHFlg = false;

	//�V�[���I�u�W�F�N�g�̐���
	Scene = new TSXScene(owner);
	//�r���[�|�[�g�̐ݒ�
	FScreenX = FOwner->GetBackBufferWidth();
	FScreenY = FOwner->GetBackBufferHeight();

	SetFOV(90);
	ClipNear = 0.01f;
	ResetProjection();

	//�t���[���̍쐬
	Root = new TSXFrame(NULL);
	Effector = new TSXFrame(Root);
	CameraStand = new TSXFrame(Root);
	Camera = new TSXFrame(CameraStand);
	Scene->SetCamera(Camera);
	FogFrame = new TSXFrame(Root);

	RX = 0;
	RY = 0;
	RZ = 0;

	//�J�����̈ʒu�E�p�������߂�
	CameraStand->SetTranslation(Root, glm::vec3(0,0,0));
	Camera->SetTranslation(CameraStand, glm::vec3(0,0,0));
	Camera->SetOrientation(Root, glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));


	//���C�g�̐ݒ�
	Lights = new TSXLightGroup(owner, 1);

	(*Lights)[0]->BeginUpdate();
	(*Lights)[0]->SetupDiffuse(1.0f,1.0f,1.0f);
	(*Lights)[0]->SetupSpecular(1.0f,1.0f,1.0f);          //0.2
	(*Lights)[0]->SetupAmbient(0.3f,0.3f,0.3f);           //0.3
	(*Lights)[0]->SetupDirectional(glm::vec3(1.f,0,0));    //0,0,1
	(*Lights)[0]->SetEnabled(true);
	(*Lights)[0]->EndUpdate();
	GlobalLight  =  (*Lights)[0];

	SetFog(0xff9f4242, 5, 10);

	//�����v�Z�Ƀ}�e���A�����g�p
	FOwner->Get3DDevice()->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, (u32)(D3DMCS_MATERIAL));
	//  Scene->SortAlpha  =  false;

	//���}�b�v�p�e�N�X�`���쐬
	InitAfterReset();

	Calc2Dto3DRatio(10.f);

}

TSceneManager::~TSceneManager()
{
	SAFE_DELETE(FogFrame);
	SAFE_DELETE(Lights);
	SAFE_DELETE(Camera);
	SAFE_DELETE(CameraStand);
	SAFE_DELETE(Effector);
	SAFE_DELETE(Root);
	SAFE_DELETE(Scene);

	ReleaseBeforeReset();


}

TSXFrame* TSceneManager::AddFrame( bool _effect )
{
	TSXFrame* frame;
	if( _effect )
		frame = new TSXFrame(Effector);
	else
		frame = new TSXFrame(Root);

	return frame;
}

void TSceneManager::Move( TSXFrame* frame, f32 _x, f32 _y, f32 _z )
{
	frame->SetTranslation(Root, glm::vec3(_x,_y,_z));

}

void TSceneManager::MoveR( TSXFrame* frame, f32 _x, f32 _y, f32 _z )
{
	frame->SetTranslation(Root, frame->GetTranslation(Root) + glm::vec3(_x,_y,_z));

}

void TSceneManager::RotateX( TSXFrame* frame, s32 _R )
{
	frame->SetTransform(frame, RotXMatrix(_R) );

}

void TSceneManager::RotateY( TSXFrame* frame, s32 _R )
{
	frame->SetTransform(frame, RotYMatrix(_R) );

}

void TSceneManager::RotateZ( TSXFrame* frame, s32 _R )
{
	frame->SetTransform(frame, RotZMatrix(_R) );

}

void TSceneManager::ZoomR( TSXFrame* frame, f32 _x, f32 _y, f32 _z )
{
	frame->SetTransform(frame, ScaleMatrix(_x,_y,_z));
	FScaleCHFlg = true;
	frame->UpdateBounds(false);

}

void TSceneManager::CameraRotate( s32 _x, s32 _y, s32 _z )
{
	RX = _x & 0xfff;
	RY = _y & 0xfff;
	RZ = _z & 0xfff;
	//    ��U�����p�x�ɖ߂�
	Camera->SetOrientation(Root,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));
	RotateY(Camera,RY);
	RotateX(Camera,RX);

	RotateZ(Camera,RZ);

}

void TSceneManager::CameraRotateR( s32 _x, s32 _y, s32 _z )
{
	RX = (RX+_x) & 0xfff;
	RY = (RY+_y) & 0xfff;
	RZ = (RZ+_z) & 0xfff;

	Camera->SetOrientation(Root,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));
	RotateY(Camera,RY);
	RotateX(Camera,RX);

	RotateZ(Camera,RZ);

}

void TSceneManager::CameraMove( f32 _x, f32 _y, f32 _z )
{
	Move(CameraStand,_x,_y,_z);

}

void TSceneManager::CameraMoveR( f32 _x, f32 _y, f32 _z )
{
	MoveR(CameraStand,_x,_y,_z);

}

void TSceneManager::SetSpriteSX( f32 _x, f32 _y, s32 _width, s32 _height, f32 _z, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend )
{
	f32 AX = 1;
	f32 AY = 1;
	D3DCOLOR col = 0x00FFFFFF | (_alpha << 24);

	f32 ScrX = (-FScreenX/2+_x*AX);
	f32 ScrY = (-FScreenY/2+_y*AY);

	TSXVertexBB VBBs[4];
	VBBs[0] = SXVertexBB(ScrX          ,ScrY           ,col, _uv.u1,_uv.v1);
	VBBs[1] = SXVertexBB(ScrX+_width*AX,ScrY           ,col, _uv.u2,_uv.v1);
	VBBs[2] = SXVertexBB(ScrX          ,ScrY+_height*AY,col, _uv.u1,_uv.v2);
	VBBs[3] = SXVertexBB(ScrX+_width*AX,ScrY+_height*AY,col, _uv.u2,_uv.v2);
	//Scene->CameraFrame
	Scene->PushBillboard(Root, glm::vec3(0,0,_z),VBBs,_tex,blend,false,true,false);


}

void TSceneManager::SetBillBoardSX( f32 _x, f32 _y, f32 _z, s32 _width, s32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend, bool _scale )
{
	SetBillBoardSX2(Root, _x, _y, _z, (f32)_width, (f32)_height,_uv,_alpha,_tex,blend, _scale,false);
}

void TSceneManager::SetBillBoardSX2( TSXFrame* _frame, f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend, bool _scale, bool _top )
{
	f32 AX = 1;
	f32 AY = 1;
	D3DCOLOR col = 0x00FFFFFF || (_alpha << 24);

	TSXVertexBB VBBs[4];
	VBBs[0] = SXVertexBB(-_width*AX/2, -_height*AY/2           ,col, _uv.u1,_uv.v1);
	VBBs[1] = SXVertexBB( _width*AX/2, -_height*AY/2           ,col, _uv.u2,_uv.v1);
	VBBs[2] = SXVertexBB(-_width*AX/2,  _height*AY/2,col, _uv.u1,_uv.v2);
	VBBs[3] = SXVertexBB( _width*AX/2,  _height*AY/2,col, _uv.u2,_uv.v2);

	Scene->PushBillboard(_frame, glm::vec3(_x,_y,_z),VBBs,_tex,blend,_scale,true,_top);

}

void TSceneManager::Render()
{
	FOwner->Get3DDevice()->SetRenderState(D3DRS_CULLMODE, DWORD(D3DCULL_CCW  ));     //�J�����OON
	Effector->SetVisibility(sxvHide);  //�G�t�F�N�g�p�t���[���͉B��
	FogFrame->SetVisibility(sxvHide);
	//�V�[���̓��e��S���`��
	Scene->Render(Root);

	FogEnable(true);
	FogFrame->SetVisibility(sxvShow);
	Scene->Render(FogFrame);
	FogEnable(false);

	Effector->SetVisibility(sxvShow);
	FScaleCHFlg = false;

}

void TSceneManager::RenderEffect()
{
	//�����_�����O�^�[�Q�b�g�ύX
	FOwner->Get3DDevice()->SetRenderTarget(0,FExRendTarget);
	Scene->SetProjection(PI*0.425f, 1.f, 0.01f, 100000.0f);
	Scene->SetViewPort(0,0,FExEffectTexture->GetWidth(), FExEffectTexture->GetHeight());
	Clear();

	//�X�P�[�����O�s�񂪎g���Ă�����A�@���𐳋K��
	//  if( FScaleCHFlg )
	//    FOwner->Get3DDevice()->SetRenderState(D3DRS_NORMALIZENORMALS,DWORD(true));

	//�G�t�F�N�g�p�V�[���̓��e��`��
	Scene->Render(Effector);
	FScaleCHFlg = false;

	//�����_�����O�^�[�Q�b�g�����ɖ߂�
	FOwner->Get3DDevice()->SetRenderTarget(0,FOwner->GetBackBuffers(0));
	ResetProjection();

}

void TSceneManager::Clear()
{
	Scene->Clear(D3DCLEAR_TARGET || D3DCLEAR_ZBUFFER , 0x00000000, 1.0f, 0);

}

TDGTexture* TSceneManager::GetExTexture()
{
	return FExEffectTexture;
}

void TSceneManager::SetEffectSprite( f32 _x, f32 _y, s32 _width, s32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend )
{
	f32 AX = (f32)FExEffectTexture->GetWidth() / 640.f;
	f32 AY = (f32)FExEffectTexture->GetHeight() / 480.f;
	D3DCOLOR col = 0x00FFFFFF || (_alpha << 24);

	f32 ScrX = (_x)*AX;
	f32 ScrY = (_y)*AY;

	TSXVertexSP VSPs[4];
	VSPs[0] = SXVertexSP(ScrX          ,ScrY           ,col, _uv.u1,_uv.v1);
	VSPs[1] = SXVertexSP(ScrX+_width*AX,ScrY           ,col, _uv.u2,_uv.v1);
	VSPs[2] = SXVertexSP(ScrX          ,ScrY+_height*AY,col, _uv.u1,_uv.v2);
	VSPs[3] = SXVertexSP(ScrX+_width*AX,ScrY+_height*AY,col, _uv.u2,_uv.v2);

	Scene->PushSprite(glm::vec2(-16,-16),VSPs,_tex,blend,true,false);  //����ɂȂ����S�~���\������邽�ߕ`��J�n�ʒu�����炵�Ă���i���}���u�j

}

void TSceneManager::ReleaseBeforeReset()
{
	SAFE_DELETE(FExRendTarget);
	SAFE_DELETE(FExEffectTexture);
}

void TSceneManager::InitAfterReset()
{
	FOwner->Get3DDevice()->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, u32(D3DMCS_MATERIAL));

	FScreenX = FOwner->GetBackBufferWidth();
	FScreenY = FOwner->GetBackBufferHeight();

	s32 TexSize;
	if( FOwner->GetBackBufferWidth()>=1024 )
		TexSize = 512;
	else if( FOwner->GetBackBufferHeight()<=512 )
		TexSize = 128;
	else
		TexSize = 256;

	FExEffectTexture = new TDGTexture(FOwner, DGFMT_ARGB,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
	FExEffectTexture->SetSize(TexSize*2,TexSize);
	FExEffectTexture->GetTexture()->GetSurfaceLevel(0, &FExRendTarget);

}

void TSceneManager::ResetProjection()
{
	Scene->SetProjection(FOV, (f32)FScreenY/ (f32)FScreenX, ClipNear, 100000.0f);
	Scene->SetViewPort(0, 0, FScreenX, FScreenY);

}

void TSceneManager::SetFog( u32 _col, f32 _start, f32 _end )
{
	FOwner->Get3DDevice()->SetRenderState( D3DRS_FOGCOLOR, _col);            // �t�H�O�J���[���w�肷��B
	FOwner->Get3DDevice()->SetRenderState( D3DRS_FOGSTART, F2DW(_start) ); // �t�H�O�̊J�n�[�x��ݒ肷��B
	FOwner->Get3DDevice()->SetRenderState( D3DRS_FOGEND, F2DW(_end) );      // �t�H�O�̏I���[�x��ݒ肷��B
	FOwner->Get3DDevice()->SetRenderState( D3DRS_FOGVERTEXMODE, u32(D3DFOG_LINEAR) );  // �t�H�O�����w�肷��B

}

void TSceneManager::FogEnable( bool _enable )
{
	FOwner->Get3DDevice()->SetRenderState( D3DRS_FOGENABLE, u32(_enable) );

}

void TSceneManager::GetCameraAngle( s32& _rx, s32& _ry, s32& _rz )
{
	_rx  =  RX;
	_ry  =  RY;
	_rz  =  RZ;

}

void TSceneManager::GetCameraPos( f32& _rx, f32& _ry, f32& _rz )
{
	glm::vec3 vec  =  CameraStand->GetTranslation(Root);

	_rx  =  vec.x;
	_ry  =  vec.y;
	_rz  =  vec.z;

}

bool TSceneManager::CheckVisible( TSXFrame* _frame )
{
	TSXVisibility visible;
	QueryVisibility(_frame, Scene, FOwner, visible);

	if( visible == sxvShow )
		return true;
	else
		return false;
}

glm::vec3 TSceneManager::Get3DPosFrom2D( f32 _x, f32 _y )
{
	glm::vec3 vec;
	vec.x  =  Ratio2Dto3D_X * (_x - 640.f * 0.5f);      //�𑜓x���ς���Ă�640,480�̂܂܂ł悢
	vec.y  =  Ratio2Dto3D_Y * (_y - 480.f * 0.5f);
	vec.z  =  Zval2Dto3D;

	vec  =  Camera->GetTranslation(Root) + TransformVec3(vec, Camera->GetMatrix());

	return vec;

}

void TSceneManager::Calc2Dto3DRatio( f32 _z )
{
	glm::mat4 viewM,projM,screenM,viewProjScreenM;

	projM = Scene->GetProjectionMatrix();
	viewM = (Camera->ViewMatrix());
	screenM = glm::mat4();
	screenM[0][0] = 640.0f / 2.0f; screenM[1][1] =  -480.f / 2.0f;
	screenM[0][3] = 640.0f / 2.0f; screenM[1][3] =  480.f / 2.0f;

	viewProjScreenM = screenM * projM * viewM;

	glm::vec3 vec  =  CameraStand->GetTranslation(Root) + TransformVec3(glm::vec3(1.f, 1.f, _z), Camera->GetMatrix());

	glm::vec4 vec2  =  Homoginize(vec);

	vec2 = viewProjScreenM * vec2;

	Ratio2Dto3D_X  =  1.f / (vec2.x / vec2.w - 640.f / 2.f);
	Ratio2Dto3D_Y  =  1.f / (vec2.y / vec2.w - 480.f / 2.f);
	Zval2Dto3D = _z;

}

void TSceneManager::SetFOV( s32 _r )
{
	FOV  =  (f32)_r / 180.f * PI;
	ResetProjection();

}

s32 TSceneManager::GetFOV()
{
	return RoundOff(FOV*180.f/PI);

}

TPoint TSceneManager::Get2DPosFrom3D( const glm::vec3& _vec )
{
	glm::mat4 viewM,projM,screenM,viewProjScreenM;

	projM = Scene->GetProjectionMatrix();
	screenM = glm::mat4();
	screenM[0][0] = 640.0f / 2.0f; screenM[1][1] =  -480.f / 2.0f;
	screenM[0][3] = 640.0f / 2.0f; screenM[1][3] =  480.f / 2.0f;
	viewM = Camera->ViewMatrix();

	viewProjScreenM = screenM * projM * viewM;

	glm::vec4 vec2  =  Homoginize(_vec);

	vec2 = viewProjScreenM * vec2;

	TPoint result;
	result.X  =  RoundOff(vec2.x/vec2.w);
	result.Y  =  RoundOff(vec2.y/vec2.w);

	return result;
}

void TSceneManager::SetClipNear( f32 _near )
{
	ClipNear = _near;
}


TFrameManager::TFrameManager( TSceneManager* owner, TFrameManager* _parent, TSXMesh* _mesh, TDGTexture* _tex, TSXBlendMode _blend, bool _light, bool _spec, bool _effect )
{
	FOwner = owner;
	if( _parent == NULL )
		Frame = owner->AddFrame(_effect);
	else
		Frame = new TSXFrame(_parent->Frame);

	RX = 0;
	RY = 0;
	RZ = 0;
	ZX = 1.0f;
	ZY = 1.0f;
	ZZ = 1.0f;
	clAlpha = 255;
	clRed = 255;
	clGreen = 255;
	clBlue = 255;
	Frame->Mesh = _mesh;
	Frame->SetTexture(_tex);
	Frame->Lighting = _light;
	Frame->Specular = _spec;
	Frame->BlendMode = _blend;

	if( Frame->Mesh != NULL ) {
		Frame->OnQueryVisibility = QueryVisibility;
		Frame->DeactivateBounds();
		Frame->OnQueryZ = QueryZ;
		Frame->UpdateBounds(false);
	}
}

TFrameManager::~TFrameManager()
{
	SAFE_DELETE(Frame);
}

void TFrameManager::Move( f32 _x, f32 _y, f32 _z )
{
	FOwner->Move(Frame,_x,_y,_z);
}

void TFrameManager::MoveR( f32 _x, f32 _y, f32 _z )
{
	FOwner->MoveR(Frame,_x,_y,_z);
}

void TFrameManager::MoveOnCamera( f32 _x, f32 _y, f32 _z )
{
	glm::vec3 dest  =  TransformVec3(glm::vec3(_x, _y, _z), ExtractRotation(FOwner->Camera->GetMatrix()));
	FOwner->Move(Frame,dest.x,dest.y,dest.z);
}

void TFrameManager::MoveOnCameraR( f32 _x, f32 _y, f32 _z )
{
	s32 _rx,_ry,_rz;
	FOwner->GetCameraAngle(_rx, _ry, _rz);
	glm::vec3 dest  =  TransformVec3(glm::vec3(_x, _y, _z), RotYMatrix(_ry));
	FOwner->MoveR(Frame,dest.x,dest.y,dest.z);

}

void TFrameManager::Rotate( s32 _RX, s32 _RY, s32 _RZ )
{
	RX = _RX & 4095;
	RY = _RY & 4095;
	RZ = _RZ & 4095;

	//���݂̊g�嗦�̕ۑ�
	f32 tmpZX = ZX;
	f32 tmpZY = ZY;
	f32 tmpZZ = ZZ;

	//��U�����p�x�ɖ߂�
	Frame->SetOrientation(FOwner->Root,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));

	FOwner->RotateX(Frame,_RX);
	FOwner->RotateY(Frame,_RY);
	FOwner->RotateZ(Frame,_RZ);

	//�����p�x�ɖ߂����ۂɃX�P�[�����O�s�������������Ă���̂ŁA�g�債�Ȃ���
	ZX = 1.f;
	ZY = 1.f;
	ZZ = 1.f;
	Zoom(tmpZX,tmpZY,tmpZZ);

}

void TFrameManager::RotateR( s32 _RX, s32 _RY, s32 _RZ )
{
	RX = (RX+_RX) & 4095;
	RY = (RY+_RY) & 4095;
	RZ = (RZ+_RZ) & 4095;

	//���݂̊g�嗦�̕ۑ�
	f32 tmpZX = ZX;
	f32 tmpZY = ZY;
	f32 tmpZZ = ZZ;


	//��U�����p�x�ɖ߂�
	Frame->SetOrientation(FOwner->Root,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));

	FOwner->RotateX(Frame,RX);
	FOwner->RotateY(Frame,RY);
	FOwner->RotateZ(Frame,RZ);

	//�����p�x�ɖ߂����ۂɃX�P�[�����O�s�������������Ă���̂ŁA�g�債�Ȃ���
	ZX = 1.f;
	ZY = 1.f;
	ZZ = 1.f;
	Zoom(tmpZX,tmpZY,tmpZZ);
}

void TFrameManager::Zoom( f32 _x,f32 _y, f32 _z )
{
	ZX = _x;
	ZY = _y;
	ZZ = _z;

	Frame->SetMeshMatrix(ScaleMatrix(ZX,ZY,ZZ));

}

void TFrameManager::ZoomR( f32 _x,f32 _y, f32 _z )
{
	ZX = ZX*_x;
	ZY = ZY*_y;
	ZZ = ZZ*_z;

	Frame->SetMeshMatrix(ScaleMatrix(ZX,ZY,ZZ));
}

void TFrameManager::SetColor( u32 _a, u32 _r, u32 _g, u32 _b )
{
	_a = _a & 255;
	_r = _r & 255;
	_g = _g & 255;
	_b = _b & 255;
	D3DCOLOR col = (_a << 24) | (_r << 16) | (_g << 8) | _b;
	TSXVertex* p = (TSXVertex*)Frame->Mesh->VBuf()->Lock(0,0,0);
	for( u32 i = 0 ; i < Frame->Mesh->VBuf()->GetVertexCount(); i++ ) {
		p->diffuse = col;
		p++;
	}
	Frame->Mesh->VBuf()->Unlock();

}

void TFrameManager::SlideUV( f32 _u, f32 _v )
{
	if( Frame->Mesh == NULL ) return;


	u8* p = (u8*)Frame->Mesh->VBuf()->Lock(0,0,0);
	for( u32 i = 0 ; i < Frame->Mesh->VBuf()->GetVertexCount(); i++ ) {
		//    //���_��uv�l���Q�b�g
		p += Frame->Mesh->VBuf()->GetVertexSize() - sizeof(f32)*2;

		*(f32*)p = *(f32*)p + _u;
		p += sizeof(f32);
		*(f32*)p = *(f32*)p + _v;
		p += sizeof(f32);
	}
	Frame->Mesh->VBuf()->Unlock();
}

void TFrameManager::Refract( f32 _s )
{
	//���b�V���̒��_�o�b�t�@�����b�N
	TSXVertex* p = (TSXVertex*)Frame->Mesh->VBuf()->Lock(0,0,0);
	for( u32 i = 0 ; i < Frame->Mesh->VBuf()->GetVertexCount(); i++ ) {
		//�@���x�N�g�����Q�b�g
		glm::vec4 NVec = glm::vec4((*p).nx, (*p).ny, (*p).nz, 1.0f);
		//���[���h���W�ɕϊ�
		NVec = Frame->GetWorldMatrix() * NVec;
		//���_�ɂ��킹�ĕϊ�
		NVec = FOwner->GetScene()->GetCamera()->ViewMatrix() * NVec;

		f32 nx = NVec.x;
		f32 ny = NVec.y;

		//���_�x�N�g���Q�b�g
		glm::vec4 VertVec = glm::vec4((*p).x, (*p).y, (*p).z, 1.0f);

		//���[���h���W���王�_�A���e�A�X�N���[�����W�ւƕϊ�
		VertVec = Frame->GetWorldMatrix() * VertVec;
		VertVec = FOwner->GetScene()->GetCamera()->ViewMatrix() * VertVec;
		VertVec = FOwner->GetScene()->GetProjectionMatrix() * VertVec;
		VertVec = FOwner->GetScene()->GetScreenMatrix() * VertVec;

		//�Ȃ�W�Ŋ���炵��
		VertVec.x = VertVec.x / VertVec.w;
		VertVec.y = VertVec.y / VertVec.w;
		VertVec.z = VertVec.z / VertVec.w;

		//���̋��܂��v�Z�i_s��0�Ȃ犮�S�ɓ����j
		u32 screenx, screeny;
		FOwner->GetScreenSize( screenx, screeny );
		(*p).tu = (VertVec.x  - nx * _s) / (f32)screenx;
		(*p).tv = (VertVec.y - ny * _s) / (f32)screeny;
		p++;
	}
	Frame->Mesh->VBuf()->Unlock();

}

void TFrameManager::Rotate2( TFrameManager* _frame, s32 _RX, s32 _RY, s32 _RZ )
{
	RX = _RX & 4095;
	RY = _RY & 4095;
	RZ = _RZ & 4095;

	//���݂̊g�嗦�̕ۑ�
	f32 tmpZX = ZX;
	f32 tmpZY = ZY;
	f32 tmpZZ = ZZ;

	//��U�����p�x�ɖ߂�
	Frame->SetOrientation(_frame->Frame,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));

	FOwner->RotateX(Frame,_RX);
	FOwner->RotateY(Frame,_RY);
	FOwner->RotateZ(Frame,_RZ);

	//�����p�x�ɖ߂����ۂɃX�P�[�����O�s�������������Ă���̂ŁA�g�債�Ȃ���
	ZX = 1.f;
	ZY = 1.f;
	ZZ = 1.f;
	Zoom(tmpZX,tmpZY,tmpZZ);

}

void TFrameManager::RotateR2( TFrameManager* _frame, s32 _RX, s32 _RY, s32 _RZ )
{
	RX = _RX & 4095;
	RY = _RY & 4095;
	RZ = _RZ & 4095;

	//���݂̊g�嗦�̕ۑ�
	f32 tmpZX = ZX;
	f32 tmpZY = ZY;
	f32 tmpZZ = ZZ;


	//��U�����p�x�ɖ߂�
	Frame->SetOrientation(_frame->Frame,  glm::vec3(0,0,1.f), glm::vec3(0,1.f,0));

	FOwner->RotateX(Frame,RX);
	FOwner->RotateY(Frame,RY);
	FOwner->RotateZ(Frame,RZ);

	//�����p�x�ɖ߂����ۂɃX�P�[�����O�s�������������Ă���̂ŁA�g�債�Ȃ���
	ZX = 1;
	ZY = 1;
	ZZ = 1;
	Zoom(tmpZX,tmpZY,tmpZZ);


}

f32 TFrameManager::GetX()
{
	glm::vec3 vec = Frame->GetTranslation(Frame->GetParent());
	return vec.x;

}

f32 TFrameManager::GetY()
{
	glm::vec3 vec = Frame->GetTranslation(Frame->GetParent());
	return vec.y;
}

f32 TFrameManager::GetZ()
{
	glm::vec3 vec = Frame->GetTranslation(Frame->GetParent());
	return vec.z;
}

f32 TFrameManager::GetZoomX()
{
	return ZX;

}

f32 TFrameManager::GetZoomY()
{
	return ZY;

}

f32 TFrameManager::GetZoomZ()
{
	return ZZ;

}

s32 TFrameManager::GetRotX()
{
	return RX;

}

s32 TFrameManager::GetRotY()
{
	return RY;

}

s32 TFrameManager::GetRotZ()
{
	return RZ;

}

bool TFrameManager::Rendered()
{
	return Frame->Rendered();

}

void QueryVisibility( TSXFrame* Sender, TSXScene* Scene, TDGCarad* DG, TSXVisibility& Visibility )
{
	if( Sender->GetVisibility() == sxvHide ) {
		Visibility  =  sxvHide;
		return;
	}

	if( (! Sender->GetEnableBounds()) && (! Sender->GetEnableMeshTransform()) ) {
		Visibility  =  sxvShow;
		return;
	}

	Visibility  =  sxvHide;
	glm::mat4 RenderedMatrix;
	if( (Sender->Mesh != NULL) ) {
		if( Sender->GetParent() == NULL )
			RenderedMatrix  =  glm::mat4();
		else
			RenderedMatrix  =  Sender->GetParent()->GetWorldMatrix();

		glm::mat4& meshmat = Sender->GetMeshMatrix();
		f32 ScaleX  =  meshmat[0][0];
		f32 ScaleY  =  meshmat[1][1];
		f32 ScaleZ  =  meshmat[2][2];


		TSphere3D sphere;
		sphere.R = glm::max(ScaleX, glm::max(ScaleY, ScaleZ)) * Sender->Mesh->GetSphere().R;
		sphere.Center = Sender->Mesh->GetSphere().Center + glm::vec3(meshmat[0][3], meshmat[1][3], meshmat[2][3]);

		if( Scene->SphereVisibility(RenderedMatrix * Sender->GetMatrix(), sphere, Scene->GetFarZ()) ) {
			Visibility = sxvShow;
		}
	}
}

void QueryZ( TSXFrame* Sender, TSXScene* Scene, TDGCarad* DG, f32& ZValue )
{
	glm::mat4 viewM,projM,screenM,viewProjScreenM;  //�r���[�s��E�ˉe�s��E�X�N���[���s�񁩂R�̍���
	D3DMATRIX tmpmat;
	Scene->GetDG()->Get3DDevice()->GetTransform(D3DTS_PROJECTION, &tmpmat);
	projM = D3DMATRIXtomat4(tmpmat);
	Scene->GetDG()->Get3DDevice()->GetTransform(D3DTS_VIEW, &tmpmat);
	viewM = D3DMATRIXtomat4(tmpmat);

	screenM = Scene->GetScreenMatrix();

	viewProjScreenM = screenM * projM * viewM;

	glm::mat4& meshmat = Sender->GetMeshMatrix();
	f32 ScaleX  =  meshmat[0][0];
	f32 ScaleY  =  meshmat[1][1];
	f32 ScaleZ  =  meshmat[2][2];

	TSphere3D sphere;
	sphere.R = glm::max(ScaleX, glm::max(ScaleY, ScaleZ)) * Sender->Mesh->GetSphere().R;

	glm::mat4 mtrx = viewProjScreenM * Sender->GetMeshMatrixOnRender();
	f32 zC = (mtrx * Homoginize(Sender->Mesh->GetSphere().Center)).w;

	ZValue = zC + sphere.R;

}