
// MFCApplication11View.cpp: CMFCApplication11View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication11.h"
#endif

#include "MFCApplication11Doc.h"
#include "MFCApplication11View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define INF 1e9 // 무한대를 나타내는 큰 값 (10억)
#endif


// CMFCApplication11View

IMPLEMENT_DYNCREATE(CMFCApplication11View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication11View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCApplication11View 생성/소멸

CMFCApplication11View::CMFCApplication11View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCApplication11View::~CMFCApplication11View()
{
}

BOOL CMFCApplication11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCApplication11View 그리기

void CMFCApplication11View::OnDraw(CDC* pDC)
{
	CMFCApplication11Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CImage mapImage;
	mapImage.Load(_T("C:\\Users\\LMS\\Desktop\\한기대\\4학년 2학기\\응용프로그램\\과제\\map.jpg"));
	mapImage.Draw(pDC->GetSafeHdc(), 0, 0);

    DrawGraph(pDC);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CMFCApplication11View::DrawGraph(CDC* pDC)
{
    CMFCApplication11Doc* pDoc = GetDocument();

    if (!pDoc) return;

    // 파란색 도구 정의
    CBrush blueBrush(RGB(0, 0, 255));
    CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));

    // 빨간색 도구 정의
    CBrush redBrush(RGB(255, 0, 0));

    // 펜과 브러시의 초기 상태를 파란색으로 설정
    CBrush* pOldBrush = pDC->SelectObject(&blueBrush);
    CPen* pOldPen = pDC->SelectObject(&bluePen);

    // ----------------------------------------------------
    // 1. 파란색 에지 그리기
    // ----------------------------------------------------
    pDC->SelectObject(&bluePen);
    for (int i = 0; i < pDoc->m_edges.GetSize(); i++)
    {
        Edge edge = pDoc->m_edges.GetAt(i);
        Node startNode = pDoc->m_nodes.GetAt(edge.startId);
        Node endNode = pDoc->m_nodes.GetAt(edge.endId);
        pDC->MoveTo(startNode.pos);
        pDC->LineTo(endNode.pos);
    }

    // ----------------------------------------------------
    // 2. 노드(점) 그리기 (출발/도착점만 빨간색으로 덮어 칠함)
    // ----------------------------------------------------

    // A. 모든 노드를 파란색으로 그리기 (기본 상태)
    pDC->SelectObject(&blueBrush);
    for (int i = 0; i < pDoc->m_nodes.GetSize(); i++)
    {
        Node node = pDoc->m_nodes.GetAt(i);
        CRect rect(node.pos.x - 5, node.pos.y - 5, node.pos.x + 5, node.pos.y + 5);
        pDC->Ellipse(&rect); // 모두 파란색으로 칠합니다.
    }

    // B. 출발 노드 빨간색 하이라이트 (Shift 클릭 시 하이라이트)
    if (pDoc->m_pathStartNodeId != -1)
    {
        int startId = pDoc->m_pathStartNodeId;
        Node startNode = pDoc->m_nodes.GetAt(startId);

        pDC->SelectObject(&redBrush); 
        CRect rect(startNode.pos.x - 5, startNode.pos.y - 5, startNode.pos.x + 5, startNode.pos.y + 5);
        pDC->Ellipse(&rect); // 파란색 위에 빨간색을 덧그립니다.
    }

    // C. 도착 노드 빨간색 하이라이트 (Shift 두 번째 클릭 시 하이라이트)
    if (pDoc->m_pathEndNodeId != -1)
    {
        int endId = pDoc->m_pathEndNodeId;
        Node endNode = pDoc->m_nodes.GetAt(endId);

        pDC->SelectObject(&redBrush); 
        CRect rect(endNode.pos.x - 5, endNode.pos.y - 5, endNode.pos.x + 5, endNode.pos.y + 5);
        pDC->Ellipse(&rect); // 파란색 위에 빨간색을 덧그립니다.
    }


    // ----------------------------------------------------
    // 3. 최단 경로 (빨간색 선) 그리기 
    // ----------------------------------------------------
    if (pDoc->m_shortestPath.GetSize() > 1)
    {
        // 파란색 펜 복원 후, 빨간색 펜 선택
        pDC->SelectObject(pOldPen);
        CPen redPen(PS_SOLID, 4, RGB(255, 0, 0));
        pOldPen = pDC->SelectObject(&redPen);

        for (int i = 0; i < pDoc->m_shortestPath.GetSize() - 1; i++)
        {
            int startId = pDoc->m_shortestPath.GetAt(i);
            int endId = pDoc->m_shortestPath.GetAt(i + 1);

            Node startNode = pDoc->m_nodes.GetAt(startId);
            Node endNode = pDoc->m_nodes.GetAt(endId);

            pDC->MoveTo(startNode.pos);
            pDC->LineTo(endNode.pos);
        }
    }

    // 4. 도구 복원 (함수 끝)
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

BOOL CMFCApplication11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCApplication11View 진단

#ifdef _DEBUG
void CMFCApplication11View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication11Doc* CMFCApplication11View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication11Doc)));
	return (CMFCApplication11Doc*)m_pDocument;
}

double CMFCApplication11View::GetDistance(CPoint p1, CPoint p2)
{
    // 유클리드 거리 공식
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

int CMFCApplication11View::FindNodeAtPoint(CPoint point)
{
    CMFCApplication11Doc* pDoc = GetDocument();
    if (!pDoc) return -1;

    const int tolerance = 15;

    for (int i = 0; i < pDoc->m_nodes.GetSize(); i++)
    {
        Node node = pDoc->m_nodes.GetAt(i);

      
        double dist = sqrt(pow(point.x - node.pos.x, 2) + pow(point.y - node.pos.y, 2));

        if (dist <= tolerance)
        {
            return node.id; 
        }
    }
    return -1;
}

// CMFCApplication11View.cpp 파일

void CMFCApplication11View::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMFCApplication11Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Alt/Ctrl/Shift 클릭에서만 필요하므로, 노드 ID 검색을 먼저 수행합니다.
    int clickedNodeId = FindNodeAtPoint(point);

    // ------------------------------------------------------------------
    // [1] Shift + 좌클릭: 최단 경로 실행 및 점 하이라이트 기능 통합
    // ------------------------------------------------------------------
    if (nFlags & MK_SHIFT)
    {
        if (clickedNodeId != -1) // 기존 노드를 클릭했을 경우에만 처리
        {
            if (pDoc->m_startNodeId == -1)
            {
                // A. 첫 번째 노드 선택 (출발 노드)
                pDoc->m_startNodeId = clickedNodeId;

                // 새로운 탐색 시작 시 기존 경로 및 점 하이라이트 초기화
                pDoc->m_shortestPath.RemoveAll();
                pDoc->m_pathEndNodeId = -1; // 이전 도착점 ID 초기화 (핵심)

                // 첫 번째 Shift 클릭한 점만 빨간색으로 하이라이트
                pDoc->m_pathStartNodeId = clickedNodeId;

                Invalidate(FALSE); // 즉시 갱신
            }
            else // B. 두 번째 노드 선택 (도착 노드) 및 Dijkstra 실행
            {
                int startId = pDoc->m_startNodeId;
                int endId = clickedNodeId;

                // 1. 기존 빨간색 경로 초기화 및 Dijkstra 실행
                pDoc->m_shortestPath.RemoveAll();
                Dijkstra(startId, endId);

                // 2. 빨간색 점 표시를 위해 ID 저장 (출발/도착 노드)
                pDoc->m_pathStartNodeId = startId;
                pDoc->m_pathEndNodeId = endId; // <--- 도착 노드 ID도 저장

                // 3. 상태 초기화 및 화면 갱신
                pDoc->m_startNodeId = -1; // 시작 노드 초기화
                Invalidate(FALSE);
            }
        }
        else
        {
            // 빈 공간 클릭 시 모든 경로/하이라이트 초기화
            pDoc->m_startNodeId = -1;
            pDoc->m_shortestPath.RemoveAll();
            pDoc->m_pathStartNodeId = -1;
            pDoc->m_pathEndNodeId = -1;
            Invalidate(FALSE);
        }

        CView::OnLButtonDown(nFlags, point);
        return; // Shift 클릭 로직 완료
    }

    // ------------------------------------------------------------------
    // [2] Ctrl + 좌클릭: 에지 연결 로직
    // ------------------------------------------------------------------
    else if (nFlags & MK_CONTROL)
    {
        // Ctrl 클릭 시 최단 경로 하이라이트 초기화
        pDoc->m_shortestPath.RemoveAll();
        pDoc->m_pathStartNodeId = -1;
        pDoc->m_pathEndNodeId = -1;

        if (clickedNodeId != -1) // 기존 노드를 클릭했을 경우에만 처리
        {
            // A. 첫 번째 노드 선택
            // pDoc->m_startNodeId가 -1이라는 것은 첫 번째 클릭이라는 뜻입니다.
            if (pDoc->m_startNodeId == -1)
            {
                pDoc->m_startNodeId = clickedNodeId; // ID 저장
                Invalidate(FALSE);
            }

            // B. 두 번째 노드 선택 및 에지 생성
            else
            {
                int endNodeId = clickedNodeId;
                int startId = pDoc->m_startNodeId; // 이전에 저장된 시작점 ID

                if (startId != endNodeId) // 시작점과 끝점이 다를 경우에만 에지 생성
                {
                    // 1. 노드 좌표 가져오기
                    Node startNode = pDoc->m_nodes.GetAt(startId);
                    Node endNode = pDoc->m_nodes.GetAt(endNodeId);

                    // 2. 거리(가중치) 계산
                    double distance = GetDistance(startNode.pos, endNode.pos);

                    // 3. 에지 생성 및 m_edges에 저장
                    Edge newEdge;
                    newEdge.startId = startId;
                    newEdge.endId = endNodeId;
                    newEdge.weight = distance;
                    pDoc->m_edges.Add(newEdge);

                    // 4. 인접 리스트 (m_adj) 업데이트 (양방향 연결)
                    Adjacency adjToTarget, adjToStart;
                    adjToTarget.targetId = endNodeId;
                    adjToTarget.weight = distance;
                    adjToStart.targetId = startId;
                    adjToStart.weight = distance;

                    pDoc->m_adj.GetAt(startId)->Add(adjToTarget);
                    pDoc->m_adj.GetAt(endNodeId)->Add(adjToStart);

                    // 5. 상태 초기화 및 화면 갱신
                    pDoc->m_startNodeId = -1; // 시작 노드 초기화 (필수!)
                    Invalidate(FALSE);
                }
                else
                {
                    // 같은 노드를 연속 클릭한 경우, 시작 노드를 초기화
                    pDoc->m_startNodeId = -1;
                }
            }
        }

        CView::OnLButtonDown(nFlags, point);
        return;
    }

    // ------------------------------------------------------------------
    // [3] 아무 키도 눌리지 않았을 때: 노드 추가 로직 (순수 좌클릭)
    // ------------------------------------------------------------------
    else
    {
        // 단순 클릭 시 모든 하이라이트 및 경로 초기화
        pDoc->m_shortestPath.RemoveAll();
        pDoc->m_pathStartNodeId = -1;
        pDoc->m_pathEndNodeId = -1;

        Node newNode;
        newNode.pos = point;
        newNode.id = pDoc->m_nextNodeId++;

        pDoc->m_nodes.Add(newNode);

        // 인접 리스트에 새 노드용 빈 리스트 추가
        CArray<Adjacency, Adjacency>* newAdjList = new CArray<Adjacency, Adjacency>;
        pDoc->m_adj.Add(newAdjList);

        Invalidate(FALSE); // 화면 갱신 요청
    }

    CView::OnLButtonDown(nFlags, point);
}

void CMFCApplication11View::Dijkstra(int startId, int endId)
{
    CMFCApplication11Doc* pDoc = GetDocument();
    int N = static_cast<int>(pDoc->m_nodes.GetSize());

    if (N == 0 || startId == -1 || endId == -1) return;

    // 1. 거리(Distance)와 이전 노드(Previous) 배열 초기화
    CArray<double, double> dist;
    CArray<int, int> prev;
    CArray<BOOL, BOOL> visited;

    // 배열 크기 설정
    dist.SetSize(N); prev.SetSize(N); visited.SetSize(N);

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = FALSE;
    }

    dist[startId] = 0; // 시작 노드의 거리는 0

    // 2. 알고리즘 실행 (N번 반복)
    for (int count = 0; count < N - 1; count++)
    {
        // A. 방문하지 않은 노드 중 가장 거리가 짧은 노드(u) 찾기
        double min_dist = INF;
        int u = -1;
        for (int i = 0; i < N; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visited[u] = TRUE;

        // B. 이웃 노드(v)에 대해 이완(Relaxation) 수행
        CArray<Adjacency, Adjacency>* pAdjList = pDoc->m_adj.GetAt(u);
        for (int i = 0; i < pAdjList->GetSize(); i++) {
            int v = pAdjList->GetAt(i).targetId;
            double weight = pAdjList->GetAt(i).weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    // 3. 최단 경로 역추적 및 결과 저장
    CArray<int, int> path;
    int current = endId;
    while (current != -1)
    {
        path.InsertAt(0, current);
        if (current == startId) break;
        current = prev[current];
    }

    // **경로 저장 및 거리 표시**
    pDoc->m_shortestPath.Copy(path);

    // 최단 거리 표시 (소수점 첫째자리)
    CString strDistance;
    if (dist[endId] < INF) {
        strDistance.Format(_T("최단 거리: %.1f 픽셀"), dist[endId]);
    }
    else {
        strDistance.Format(_T("경로를 찾을 수 없습니다."));
    }
    AfxMessageBox(strDistance);
}
#endif //_DEBUG


// CMFCApplication11View 메시지 처리기
