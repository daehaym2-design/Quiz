
// MFCApplication11Doc.h: CMFCApplication11Doc 클래스의 인터페이스
//


#pragma once

struct Node {
	CPoint pos;
	int id = 0; 
};
struct Edge {
	int startId;
	int endId;
	double weight; // 두 노드 사이의 거리(가중치)
};
struct Adjacency {
	int targetId;   // 연결된 노드의 ID
	double weight;  // 노드까지의 거리(가중치)
};

class CMFCApplication11Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication11Doc() noexcept;
	DECLARE_DYNCREATE(CMFCApplication11Doc)

// 특성입니다.
public:
	CArray<Node, Node> m_nodes;     
	int m_nextNodeId = 0;
	CArray<Edge, Edge> m_edges;              // 에지(선) 목록
	CArray<CArray<Adjacency, Adjacency>*, CArray<Adjacency, Adjacency>*> m_adj;
	int m_startNodeId = -1; // 에지나 경로 찾기의 시작 노드 ID (-1: 선택 안 됨)
	CArray<int, int> m_shortestPath;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	int m_shiftHighlightId = -1;
	int m_highlightNodeId = -1;
	// 최단 경로의 출발 노드 ID (빨간색 점 표시용)
	int m_pathStartNodeId = -1;
	// 최단 경로의 도착 노드 ID (빨간색 점 표시용)
	int m_pathEndNodeId = -1;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMFCApplication11Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
