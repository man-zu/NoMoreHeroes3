#include "VIBuffer_UI_Rect_Instance.h"

CVIBuffer_UI_Rect_Instance::CVIBuffer_UI_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance { pDevice, pContext }
{
}

CVIBuffer_UI_Rect_Instance::CVIBuffer_UI_Rect_Instance(const CVIBuffer_UI_Rect_Instance & rhs)
	: CVIBuffer_Instance (rhs)
	, m_pInstanceVertices { rhs.m_pInstanceVertices }
	, m_iIdx{rhs.m_iIdx}
	, m_iNumCol{rhs.m_iNumCol}
	, m_iNumRow{rhs.m_iNumRow}
	, m_pInstanceVertexPos(rhs.m_pInstanceVertexPos)
	, m_fSpeed(rhs.m_fSpeed)
	, m_iAlphaAreaTotCnt(rhs.m_iAlphaAreaTotCnt)
	, m_fScale(rhs.m_fScale)
{
	m_bSign = new _bool[m_iNumCol];
	m_iSign = new _int[m_iNumCol];
}

HRESULT CVIBuffer_UI_Rect_Instance::Initialize(void* pArg)
{

#pragma region INSTANCE_BUFFER

	Safe_Release(m_pVBInstance);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iInstanceVertexStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceVertexStride;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pInstanceVertices;

	m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_UI_Rect_Instance::Initialize_Prototype(const UIINSTANCE_DESC* pInstanceDesc)
{
	m_iNumRow = pInstanceDesc->iRow;
	m_iNumCol = pInstanceDesc->iCol;
	m_fSpeed = pInstanceDesc->fSpeed;
	m_iNumInstance = m_iNumRow * m_iNumCol;

	m_iNumIndexPerInstance = 6;
	m_iStride = sizeof(VTXTEX);
	m_iInstanceVertexStride = sizeof(VTXUIPOSTEX);
	m_iNumVertices = 4;
	m_iIndexStride = 2;
	m_iNumIndices = m_iNumIndexPerInstance * m_iNumInstance;
	m_iNumVertexBuffers = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	/* 생성하고자하는 버퍼의 속성을 설정한다. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	/* 생성하고자하는 버퍼의 초기값을 설정한다. */
	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	pVertices[0].vPosition = XMFLOAT3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = XMFLOAT2(0.f, 0.f);

	pVertices[1].vPosition = XMFLOAT3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = XMFLOAT2(1.f, 0.f);

	pVertices[2].vPosition = XMFLOAT3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = XMFLOAT2(1.f, 1.f);

	pVertices[3].vPosition = XMFLOAT3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexture = XMFLOAT2(0.f, 1.f);

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint		iNumIndices = {};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 1;
		pIndices[iNumIndices++] = 2;

		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 2;
		pIndices[iNumIndices++] = 3;
	}

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	m_pInstanceVertices = new VTXUIPOSTEX[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTXUIPOSTEX) * m_iNumInstance);

	/* 다시 처음위치로 돌아갈 수 있도록 초기위치를 저장하기위한 공간을 할당하자. */
	m_pInstanceVertexPos = new _float4[m_iNumInstance];
	ZeroMemory(m_pInstanceVertexPos, sizeof(_float4) * m_iNumInstance);

	m_fScale = pInstanceDesc->fScale;

	// 좌상단 (0, 0)
	if (pInstanceDesc->eAlign == UIALIGN::UI_LT)
	{
		for (_int i = 0; i < m_iNumCol; i++)
		{
			for (_int j = 0; j < m_iNumRow; j++)
			{
				m_iIdx = m_iNumRow * i + j;

				m_pInstanceVertices[m_iIdx].vRight = _float4(pInstanceDesc->fScale, 0.f, 0.f, 0.f);
				m_pInstanceVertices[m_iIdx].vUp = _float4(0.f, pInstanceDesc->fScale, 0.f, 0.f);
				m_pInstanceVertices[m_iIdx].vLook = _float4(0.f, 0.f, pInstanceDesc->fScale, 0.f);
				m_pInstanceVertices[m_iIdx].vTranslation = _float4(
					pInstanceDesc->fMargin * _float(j),
					pInstanceDesc->fMargin * _float(i) * (-1.f),
					0.f,
					1.f);

				if (1 == pInstanceDesc->vColorAlpha[m_iIdx])
					++m_iAlphaAreaTotCnt;

				m_pInstanceVertices[m_iIdx].vColorAlpha = pInstanceDesc->vColorAlpha[m_iIdx];
				m_pInstanceVertices[m_iIdx].vColor = pInstanceDesc->vColor;
				m_pInstanceVertices[m_iIdx].vColorOff = 0;

				m_pInstanceVertexPos[m_iIdx] = m_pInstanceVertices[m_iIdx].vTranslation;
			}
		}
	}

	// 우상단 (0, 0)
	if (pInstanceDesc->eAlign == UIALIGN::UI_RT)
	{
		for (_int i = 0; i < m_iNumRow; i++)
		{
			for (_int j = 0; j < m_iNumCol; j++)
			{
				m_iIdx = m_iNumCol * i + j;

				m_pInstanceVertices[m_iIdx].vRight = _float4(pInstanceDesc->fScale, 0.f, 0.f, 0.f);
				m_pInstanceVertices[m_iIdx].vUp = _float4(0.f, pInstanceDesc->fScale, 0.f, 0.f);
				m_pInstanceVertices[m_iIdx].vLook = _float4(0.f, 0.f, pInstanceDesc->fScale, 0.f);
				m_pInstanceVertices[m_iIdx].vTranslation = _float4(
					pInstanceDesc->fMargin * _float(i) * (-1.f),
					pInstanceDesc->fMargin * _float(j) * (-1.f),
					0.f,
					1.f);

				if (1 == pInstanceDesc->vColorAlpha[m_iIdx])
					++m_iAlphaAreaTotCnt;

				m_pInstanceVertices[m_iIdx].vColorAlpha = pInstanceDesc->vColorAlpha[m_iIdx];
				m_pInstanceVertices[m_iIdx].vColor = pInstanceDesc->vColor;
				m_pInstanceVertices[m_iIdx].vColorOff = 0;

				m_pInstanceVertexPos[m_iIdx] = m_pInstanceVertices[m_iIdx].vTranslation;
			}
		}
	}

	return S_OK;
}

HRESULT CVIBuffer_UI_Rect_Instance::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance,
	};

	_uint					iStrides[] = {
		m_iStride,
		m_iInstanceVertexStride
	};

	_uint					iOffsets[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CVIBuffer_UI_Rect_Instance::Update_Down(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{		
		VTXMATRIX*		pVertices = static_cast<VTXMATRIX*>(SubResource.pData) + i;		

		_vector		vDir = XMVectorSet(0.f, -1.f, 0.f, 0.f);

		XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * m_fSpeed * fTimeDelta);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::Beat(_float fTimeDelta, _float fDuringTime)
{
	/* Col MustBe odd number */
	_float fMinus = 1.f;

	m_fTimeDelay += fTimeDelta;
	if (m_fTimeDelay >= fDuringTime)
	{
		m_bMoved = !m_bMoved;
		m_fTimeDelay = 0.f;
	}

	if (m_bMoved)
		fMinus = -1.f;


	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_vector		vDir = {};
			_uint		iIdx = 0;
			_uint		iRange = 0;
			_uint		iMoveCnt = 0;	// 움직여야하는 총 횟수
			_uint		iComparIdx = 0;

			iIdx = iRow + iCol * m_iNumRow;
			iComparIdx = _uint(iIdx - (iCol * m_iNumCol));

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			/* Dir: TOP */
			vDir = { 0.f, 1.f, 0.f, 0.f };
			iRange = _uint(m_iNumCol / 2) * m_iNumRow;
			if (iIdx < iRange)
			{
				XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fMinus * m_fSpeed * fTimeDelta);
			}

			/* Dir: BOTTOM */
			vDir = { 0.f, -1.f, 0.f, 0.f };
			iRange = (_uint(m_iNumCol / 2) + 1) * m_iNumRow;
			if (iIdx >= iRange)
			{
				XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fMinus * m_fSpeed * fTimeDelta);
			}


			iRange = _uint(m_iNumRow / 2);
			/* Dir: RIGHT */
			vDir = { 1.f, 0.f, 0.f, 0.f };
			if (iRange < iComparIdx)
			{
				iMoveCnt = _uint(iIdx - (iCol * m_iNumCol)) - iRange;
				for (_uint i = 0; i < iMoveCnt; ++i)
					XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fMinus * m_fSpeed * fTimeDelta);
			}

			/* Dir: LEFT */
			vDir = { -1.f, 0.f, 0.f, 0.f };
			if (iRange > iComparIdx)
			{
				iMoveCnt = iRange - _uint(iIdx - (iCol * m_iNumCol));
				for (_uint i = 0; i < iMoveCnt; ++i)
					XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fMinus * m_fSpeed * fTimeDelta);
			}

		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::DirMove(_float fDelayTime, _float fDuringTime, INSUI_MOVETYPE eBeatType)
{
	/*
	 * 4개의 MiniSkillIcon 움직임을 동일하게 하기 위해서
	 * (다른 함수와 다르게) fDelayTime 을 인자로 받고 있음.
	 */
	if (fDelayTime >= fDuringTime)
	{
		if (m_bMoved)
		{
			m_bMoved = false;

			D3D11_MAPPED_SUBRESOURCE		SubResource{};
			m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

			for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
			{
				for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
				{
					_int iIdx = iRow + iCol * m_iNumRow;

					VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
					if (0 == pVertices->vColorAlpha)
						continue;

					_vector vCurPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]);
					XMStoreFloat4(&pVertices->vTranslation, vCurPos);
				}
			}
			m_pContext->Unmap(m_pVBInstance, 0);
		}

		return;
	}

	m_bMoved = true;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_vector		vDir = {};
			_int		iIdx = 0;
			_int		iRange = 0;
			_int		iComparIdx = 0;

			iIdx = iRow + iCol * m_iNumRow;
			iComparIdx = _int(iIdx - (iCol * m_iNumCol));

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			_vector vCurPos = XMLoadFloat4(&pVertices->vTranslation);
			_vector vTargetPos = {};
			_vector vMoveAmount = {};

			/* Dir: TOP */
			if(eBeatType == INSUI_MOVE_TOP)
			{
				vDir = { 0.f, 1.f, 0.f, 0.f };
				iRange = m_iNumCol - 1;
				if (iCol < iRange)
					for (_int i = 0; i < (m_iNumCol - 1 - iCol); ++i)
						vMoveAmount += XMVector3Normalize(vDir);
			}
			/* Dir: BOTTOM */
			else if(eBeatType == INSUI_MOVE_BOTTOM)
			{
				vDir = { 0.f, -1.f, 0.f, 0.f };
				if (iCol > iRange)
					vMoveAmount += XMVector3Normalize(vDir) * (_float)iCol;
			}
			/* Dir: RIGHT */
			else if(eBeatType == INSUI_MOVE_RIGHT)
			{
				vDir = { 1.f, 0.f, 0.f, 0.f };
				if (iRange < iRow)
					vMoveAmount += XMVector3Normalize(vDir) * (_float)iRow;
			}
			/* Dir: LEFT */
			else if(eBeatType == INSUI_MOVE_LEFT)
			{
				vDir = { -1.f, 0.f, 0.f, 0.f };
				iRange = 2;
				if (iRange > iRow)
					for (_int i = 1; i <= (m_iNumRow - 1 - iRow); ++i)
						vMoveAmount += XMVector3Normalize(vDir) * (_float)i;
			}


			vTargetPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]) + vMoveAmount;
			XMVECTOR result = XMVectorLerp(vCurPos, vTargetPos, .3f);
			XMStoreFloat4(&pVertices->vTranslation, result);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::Wave(_float fTimeDelta)
{
	/*
	* 위로 이동하면서 커졌다가 제자리로 원래 사이즈로
	* 
	*/

	/* 최초 진입 */
	if (!m_bWave)
	{
		m_bWave = true;

		ZeroMemory(m_bSign, sizeof(_bool) * m_iNumCol);
		ZeroMemory(m_iSign, sizeof(_int) * m_iNumCol);

		return;
	}

	m_fTimeDelay += fTimeDelta;
	if (m_fTimeDelay > m_iNumCol * 2.f)
	{
		/* 상태 초기화 */
		m_fTimeDelay = 0.f;

		ZeroMemory(m_bSign, sizeof(_bool) * m_iNumCol);
		ZeroMemory(m_iSign, sizeof(_int) * m_iNumCol);
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0.f == pVertices->vColorAlpha)
				continue;

			if (!m_bSign[iCol] && pVertices->vRight.x >= 10.f)
			{
				m_bSign[iCol] = true;
				++m_iSign[iCol];
			}
			if (m_bSign[iCol] && pVertices->vRight.x <= m_fScale)
			{
				m_bSign[iCol] = false;
				++m_iSign[iCol];
			}

			if (m_fTimeDelay < iCol)
				continue;

			if (m_fTimeDelay > iCol && (m_iSign[iCol] > 1 && !m_bSign[iCol]))
			{
				/* 한 번 웨이브 하면 원래자리 유지 */
				pVertices->vRight = _float4(m_fScale, 0.f, 0.f, 0.f);
				pVertices->vUp = _float4(0.f, m_fScale, 0.f, 0.f);
				pVertices->vLook = _float4(0.f, 0.f, m_fScale, 0.f);
				memcpy(&pVertices->vTranslation, &m_pInstanceVertexPos[iIdx], sizeof(_float4));

				continue;
			}

			/*
			* [ 웨이브 패턴 ]
			* 크기 커질땐 뒤로 이동(m_bSign : TRUE),
			* 다시 원래 사이즈로 돌아갈때 앞으로 이동 (m_bSign : FALSE)
			*/
			_float fSignTimeDelta = fTimeDelta * 0.4f;
			fSignTimeDelta *= m_bSign[iCol] ? -1.f : 1.f;

			pVertices->vRight = _float4(pVertices->vRight.x + fSignTimeDelta, 0.f, 0.f, 0.f);
			pVertices->vUp = _float4(0.f, pVertices->vUp.y + fSignTimeDelta, 0.f, 0.f);
			pVertices->vLook = _float4(0.f, 0.f, pVertices->vLook.z + fSignTimeDelta, 0.f);
			
			
			_vector		vDir = {};
			_int		iRange = 0;
			_int		iMoveCnt = 0;	// 움직여야하는 총 횟수

			iRange = _int(m_iNumRow / 2);
			/* Dir: RIGHT */
			vDir = { 1.f, 0.f, 0.f, 0.f };
			if (iRange < iRow)
			{
				iMoveCnt = iRow - iRange;
				for (_int i = 0; i < iMoveCnt; ++i)
					XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * (fSignTimeDelta));
			}

			/* Dir: LEFT */
			vDir = { -1.f, 0.f, 0.f, 0.f };
			if (iRange > iRow)
			{
				iMoveCnt = iRange - iRow;
				for (_int i = 0; i <= iMoveCnt; ++i)
					XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * (fSignTimeDelta));
			}

			/* Dir: Row 전체Y이동 */
			vDir = { 0.f, 1.f, 0.f, 0.f };
			XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * (fSignTimeDelta * 5.f));
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::SpreadIn(_float fTimeDelta, _float fDuringTime, _float fSize, _float fSpeed, _uint* eParam)
{
	/*
	* col 속도를 cos 값이용 && row 홀/짝 이용
	* 
	*/
	/* Off 하고 다시 on 하는 경우 최초 위치 돌려줘야함 */

	_bool bIsEnd = false;
	_int iFalseCnt = 0;

	m_fTimeDelay += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;
			_int iColChg = 0;

			if (m_iNumCol > 10)
				iColChg = (_int)iCol * 5;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			_float fDisplacement = cos(iColChg + m_fTimeDelay * fSpeed) * fSize;
			_vector vDir = { 1.f, 0.f, 0.f , 0.f };
			if (iRow % 2 == 0)
				vDir = { -1.f, 0.f, 0.f , 0.f };

			if (m_fTimeDelay == fTimeDelta || fDuringTime <= m_fTimeDelay)
			{
				_vector vCurPos = XMLoadFloat4(&pVertices->vTranslation);
				_vector vTargetPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]);
				XMVECTOR result = XMVectorLerp(vCurPos, vTargetPos, .3f);
				XMStoreFloat4(&pVertices->vTranslation, result);

				if (0 == iFalseCnt && abs(XMVector4Length(vTargetPos - vCurPos).m128_f32[0]) < 0.0001f)
					bIsEnd = true;
				else
				{
					/* 한 번이라도 false 이였다면, true X */
					bIsEnd = false;
					iFalseCnt++;
				}
			}
			else
			{
				pVertices->vRight = _float4(m_fScale, 0.f, 0.f, 0.f);
				pVertices->vUp = _float4(0.f, m_fScale, 0.f, 0.f);
				pVertices->vLook = _float4(0.f, 0.f, m_fScale, 0.f);
				XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fDisplacement);
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);

	if (bIsEnd && m_fTimeDelay != fTimeDelta)
	{
		m_fTimeDelay = 0.f;
		*eParam = 0;
	}
}

void CVIBuffer_UI_Rect_Instance::SpreadOut(_float fTimeDelta, _float fSize, _float fSpeed, _uint* eParam)
{
	_bool bIsEnd = true;
	_float fSizeChange;

	m_fTimeDelay += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;
			_int iColChg = 0;

			if (m_iNumCol > 10)
				iColChg = (_int)iCol / 10 * 90 + iCol;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			_float fDisplacement = cos(iColChg + m_fTimeDelay * fSpeed) * fSize;
			fSizeChange = m_fScale - m_fTimeDelay * fSpeed;

			_vector vDir = { 1.f, 0.f, 0.f , 0.f };
			if (iRow % 2 == 0)
				vDir = { -1.f, 0.f, 0.f , 0.f };

			if (fSizeChange > 0.f)
			{
				bIsEnd = false;

				pVertices->vRight = _float4(fSizeChange, 0.f, 0.f, 0.f);
				pVertices->vUp = _float4(0.f, fSizeChange, 0.f, 0.f);
				pVertices->vLook = _float4(0.f, 0.f, fSizeChange, 0.f);
				XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * fDisplacement);
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);

	if (bIsEnd || 0.1f >= fSizeChange)
	{
		m_fTimeDelay = 0.f;
		*eParam = 0;
	}
}

void CVIBuffer_UI_Rect_Instance::MoveCircle(_float fTimeDelta)
{
	m_fTimeDelay += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;

			if (0 == pVertices->vColorAlpha)
				continue;

			_int iRand = rand() % 40;

		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::MoveIn(_float fTimeDelta, _uint* eParam)
{
	/* 최초 진입, 상단 위치로 초기화 */
	if (!m_bIn)
	{
		m_bIn = true;

		ZeroMemory(m_bSign, sizeof(_bool) * m_iNumCol);

		D3D11_MAPPED_SUBRESOURCE		SubResource{};
		m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
		{
			for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
			{
				_int iIdx = iRow + iCol * m_iNumRow;
				_int iMiddle = (_int)(m_iNumRow * 0.5f);
				_float iMargin = 80.f;

				VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
				if (0 == pVertices->vColorAlpha)
					continue;

				if (iMiddle > iRow)
					iMargin *= -1.f;

				_vector vCurPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]);
				vCurPos.m128_f32[0] += iMargin * _float(abs(iMiddle - iRow));
				vCurPos.m128_f32[1] += 1000.f;

				XMStoreFloat4(&pVertices->vTranslation, vCurPos);
			}
		}
		m_pContext->Unmap(m_pVBInstance, 0);

		return;
	}

	/* 초기화 후 이동패턴 */
	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{

		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			_vector vCurPos = XMLoadFloat4(&pVertices->vTranslation);
			_vector vTargetPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]);

			XMVECTOR result = XMVectorLerp(vCurPos, vTargetPos, 0.25f);
			XMStoreFloat4(&pVertices->vTranslation, result);

			if (iCol == m_iNumCol - 1)
			{
				if (XMVector4Length(vTargetPos - vCurPos).m128_f32[0] < 0.01f)
					m_bSign[iCol] = true;
			}

			else if (!m_bSign[iCol] && XMVector4Length(vTargetPos - vCurPos).m128_f32[0] < 900.f)
			{
				m_bSign[iCol] = true;
			}
		}

		if (!m_bSign[iCol])
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);

	if (m_bSign[m_iNumCol - 1])
	{
		m_bIn = false;
		m_bOut = false;
		m_bWave = false;
		*eParam = 0;
	}

}

void CVIBuffer_UI_Rect_Instance::MoveOut(_float fTimeDelta)
{
	/*
	* 끝난순간 m_ptransform 위치 옮기고(Pattern1)
	* 가장 마지막 행부터 불규칙한 속도로 나가기(Pattern2)
	*/

	/* 최초 진입, 초기화 */
	if (!m_bOut)
	{
		m_bOut = true;
		m_fTimeDelay = 0.f;
		ZeroMemory(m_bSign, sizeof(_bool) * m_iNumCol);
		return;
	}

	/*
	* Pattern1. 화살표 방향으로 한 번 이동
	*/
	m_fTimeDelay += fTimeDelta;
	if (m_fTimeDelay <= .5f)
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource{};
		m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
		{
			for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
			{
				_int iIdx = iRow + iCol * m_iNumRow;

				VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
				if (0 == pVertices->vColorAlpha)
					continue;

				_int iRand = rand() % 10;

				pVertices->vRight = _float4(m_fScale, 0.f, 0.f, 0.f);
				pVertices->vUp = _float4(0.f, m_fScale, 0.f, 0.f);
				pVertices->vLook = _float4(0.f, 0.f, m_fScale, 0.f);

				_vector vCurPos = XMLoadFloat4(&pVertices->vTranslation);
				_vector vTargetPos = XMLoadFloat4(&m_pInstanceVertexPos[iIdx]);
				vTargetPos.m128_f32[1] -= 100.f;

				XMVECTOR result = XMVectorLerp(vCurPos, vTargetPos, 0.01f * iRand);
				XMStoreFloat4(&pVertices->vTranslation, result);

			}
		}
		m_pContext->Unmap(m_pVBInstance, 0);
		return;
	}

	/*
	* Pattern2. 행별로 랜덤속도로 나가기
	*/
	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = (m_iNumCol-1); iCol >= 0; --iCol)
	{
		if (!m_bSign[iCol])
		{
			m_bSign[iCol] = true;
			m_pContext->Unmap(m_pVBInstance, 0);
			return;
		}

		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iCol * m_iNumRow + iRow;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			_int iRand = rand() % 50;

			_vector vDir = { 0.f, -1.f, 0.f, 0.f };
			XMStoreFloat4(&pVertices->vTranslation, XMLoadFloat4(&pVertices->vTranslation) + XMVector3Normalize(vDir) * (_float)iRand);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::ColorDecrease(_float fTimeDelta, _uint iMaxData, _uint iCurData, _uint* iColorStatus)
{
	/*
	* 감소할 영역 밝아짐 ( [CLIENT] CUI_InstanceBase::INSUI_COLOR INSUI_READY )
	* 일정 시간 지나면 감소할 영역 쪼개지면서(렉트 하나당 미니렉트 4개) 하나씩 줄어듬
	*/
	if (iColorStatus == nullptr)
	{
		MSG_BOX(L"iColorStatue param is NULLPTR");
		return;
	}

	//m_iAlphaAreaTotCnt : curRect = iMaxData : iCurData
	_int iRectTot = m_iAlphaAreaTotCnt;
	_int iBlackArea = _int((_float)(iMaxData - iCurData) / iMaxData * m_iAlphaAreaTotCnt);
	_int iRectChgAmt = iBlackArea - m_iAmountTot;		// 연한색으로 변해야 하는 양

	if (0 == iRectChgAmt && 0 == iCurData)
	{
		*iColorStatus = 0;
		return;
	}

	/* 0: colorY(기본색상) , 1: brightColor, 2: BlackColor */
	_int ChangeColor = (_int)INSUI_COLOR::INSUI_BASE;

	if (m_iAmountDec < iRectChgAmt)
		ChangeColor = (_int)INSUI_COLOR::INSUI_READY_BRIGHT;
	else 
	{
		m_fTimeDelayColor += fTimeDelta;

		if (m_fTimeDelayColor <= 0.3f)
			return;

		ChangeColor = (_int)INSUI_COLOR::INSUI_BLACK;
		m_iBlackArea = 0;
		m_iAlphaArea = 0;
	}


	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int iIdx = iRow + iCol * m_iNumRow;

			if (ChangeColor == 1 && m_iAmountDec == iRectChgAmt)
			{
				m_pContext->Unmap(m_pVBInstance, 0);
				m_iBlackArea = 0;
				return;
			}

			if (2 == ChangeColor)
			{
				if (m_iBlackArea >= iBlackArea)
				{
					m_iAmountDec = 0;
					m_iBlackArea = 0;
					m_iAlphaArea = 0;
					m_fTimeDelayColor = 0.f;
					*iColorStatus = 0;

					m_pContext->Unmap(m_pVBInstance, 0);

					return;
				}
			}

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;

			/* vColorAlpha == 0. 그려지지 않는 영역 */
			if (0 == pVertices->vColorAlpha)
				++m_iAlphaArea;

			/* vColorAlpha == 1. 그려지는 영역 */
			else if (1 == pVertices->vColorAlpha)
			{
				if (2 == pVertices->vColorOff)
				{
					++m_iBlackArea;
					continue;
				}

				if (pVertices->vColorOff == ChangeColor)
					continue;


				if (1 == ChangeColor)
				{
					if (iIdx >= m_iAlphaArea + iBlackArea - iRectChgAmt)
					{
						pVertices->vColorOff = ChangeColor;
						++m_iAmountDec;		// m_iAmountDec = 연한색으로 바뀐 횟수. 감소한 양.
					}
				}

				if (2 == ChangeColor)
				{
					pVertices->vColorOff = ChangeColor;

					++m_iAmountTot;

					m_pContext->Unmap(m_pVBInstance, 0);
					return;

				}
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::ColorIncrease(_float fTimeDelta, _uint iMaxData, _uint iCurData, _uint* iColorStatus)
{
	/*
	* 모양 커졌다 증가 끝나면 원래사이즈로 돌아감
	*/ 

	if (iColorStatus == nullptr)
	{
		MSG_BOX(L"iColorStatue param is NULLPTR");
		return;
	}

	if (iMaxData < iCurData)
		return;

	//m_iAlphaAreaTotCnt : curRect = iMaxData : iCurData
	_int iRectTot = m_iAlphaAreaTotCnt;		// 색상 활성화 영역
	_int iBlackArea = _uint((_float)(iMaxData - iCurData) / iMaxData * m_iAlphaAreaTotCnt);
	_int iRectChgAmt = m_iAmountTot - iBlackArea;

	if (0 > iRectChgAmt)
		return;

	/*
	 * 1. 뒤에서 서칭해서 블랙영역 렉트 키우고,
	 * 2. iRectChgAmt 와 m_iAmountChg 동일하면 원래 사이즈로
	 */

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int	iIdx = (m_iNumCol * m_iNumRow) - (iRow + iCol * m_iNumRow) - 1;

			VTXUIPOSTEX* pBeforeVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + m_iLastChgIdx;

			pBeforeVertices->vRight = _float4(m_fScale, 0.f, 0.f, 0.f);
			pBeforeVertices->vUp = _float4(0.f, m_fScale, 0.f, 0.f);
			pBeforeVertices->vLook = _float4(0.f, 0.f, m_fScale, 0.f);

			if (0 == m_iAmountTot || 0 >= iRectChgAmt )
			{
				m_pContext->Unmap(m_pVBInstance, 0);
				m_iLastChgIdx = 0;
				*iColorStatus = 0;

				return;
			}


			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			if (1 == pVertices->vColorAlpha && 2 == pVertices->vColorOff)
			{
				pVertices->vRight = _float4(pVertices->vRight.x * 1.2f, 0.f, 0.f, 0.f);
				pVertices->vUp = _float4(0.f, pVertices->vUp.y * 1.2f, 0.f, 0.f);
				pVertices->vLook = _float4(0.f, 0.f, pVertices->vLook.z * 1.2f, 0.f);

				pVertices->vColorOff = (_int)INSUI_COLOR::INSUI_BASE;
				m_iLastChgIdx = iIdx;
				--m_iAmountTot;

				m_pContext->Unmap(m_pVBInstance, 0);

				return;
			}

		}
	}


}

void CVIBuffer_UI_Rect_Instance::ColorRowSkillState(_float fTimeDelta, _uint iMaxData, _uint iCurData, _uint iColorStatus)
{
	/*
	* 미니스킬: 30 / 60 / 100 % 일 경우 게이지 색상 Base, 그 외 black
	* 스킬: 100% 일 경우 모든 색상 ON, 배경색은 %만큼 Row INSUI_READY_DARK 점등. 100% 일 경우 Base 색상
	*/
	if (iCurData >= iMaxData)
		iCurData = iMaxData;


	_float fGauge = (_float)iCurData / iMaxData * m_iNumCol;
	_int ColorOnIdx = m_iNumCol - (_int)floor(fGauge);



	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_int iCol = 0; iCol < m_iNumCol; ++iCol)
	{
		for (_int iRow = 0; iRow < m_iNumRow; ++iRow)
		{
			_int	iIdx = iRow + iCol * m_iNumRow;

			VTXUIPOSTEX* pVertices = static_cast<VTXUIPOSTEX*>(SubResource.pData) + iIdx;
			if (0 == pVertices->vColorAlpha)
				continue;

			if(fGauge == m_iNumCol)
				pVertices->vColorOff = (_int)INSUI_COLOR::INSUI_BASE;

			else if(iColorStatus == 4 || iCol < ColorOnIdx)		// iColorStatus 4 == INSUI_STAT_SKILLICON....
				pVertices->vColorOff = (_int)INSUI_COLOR::INSUI_BLACK;
			else
				pVertices->vColorOff = (_int)INSUI_COLOR::INSUI_READY_DARK;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_UI_Rect_Instance::ResetData()
{
	m_fTimeDelay = 0.f;
}

CVIBuffer_UI_Rect_Instance * CVIBuffer_UI_Rect_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const UIINSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_UI_Rect_Instance*		pInstance = new CVIBuffer_UI_Rect_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pInstanceDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_UI_Rect_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_UI_Rect_Instance::Clone(void * pArg)
{
	CVIBuffer_UI_Rect_Instance*		pInstance = new CVIBuffer_UI_Rect_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_UI_Rect_Instance::Free()
{
	__super::Free();

	if (false == m_bIsCloned)
	{
		Safe_Delete_Array(m_pInstanceVertices);
		Safe_Delete_Array(m_pInstanceVertexPos);

	}
	Safe_Delete_Array(m_bSign);
	Safe_Delete_Array(m_iSign);
	
}
