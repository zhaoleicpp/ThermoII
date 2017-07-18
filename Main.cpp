#include "stdafx.h"
#include "Main.h"
#include "UserInterface.h"
#include "Ray.h"

CMain g_main;

CMain::CMain(void)
{
}


CMain::~CMain(void)
{
}

void CMain::Go()
{
	m_ellipse_mirror.m_x=m_ellipse_mirror.m_y=0;
	m_ellipse_mirror.Set(211,149);
	m_left_source.m_size=19;
	m_left_source.m_x=-m_ellipse_mirror.m_c;
	m_left_source.m_y=0;
	m_left_source.m_send_rate=1;
	m_right_source.m_size=7;
	m_right_source.m_x=m_ellipse_mirror.m_c;
	m_right_source.m_y=0;
	m_right_source.m_send_rate=1;
	m_left_screen.Set(m_left_source);
	m_right_screen.Set(m_right_source);

	g_user_interface->m_left_x=-m_ellipse_mirror.m_a*1.01;
	g_user_interface->m_right_x=m_ellipse_mirror.m_a*1.01;
	g_user_interface->m_up_y=m_ellipse_mirror.m_b*1.01;
	g_user_interface->m_down_y=-m_ellipse_mirror.m_b*1.01;
	g_user_interface->UpdateSize();
	g_user_interface->PreparePaint();

	m_ellipse_mirror.Paint();
	//m_left_source.Paint();
	//m_right_source.Paint();

	double send_array[2],receive_array[2];
	send_array[0]=0;
	send_array[1]=0;
	receive_array[0]=0;
	receive_array[1]=0;
	double skip;
	skip=0;
	CRoundSource * const source_array[2]={&m_left_source,&m_right_source};
	CRoundScreen * const screen_array[2]={&m_left_screen,&m_right_screen};
	int max_loop_side,max_loop_ray_index,max_loop;
	max_loop=0;
	for(int left_right=0;left_right<2;left_right++)
	{
		double &send=send_array[left_right];
		double &receive=receive_array[left_right];
		double &the_other_receive=receive_array[(left_right+1)%2];
		CRoundSource &source=*source_array[left_right];
		CRoundScreen &screen=*screen_array[left_right];
		CRoundScreen &the_other_screen=*screen_array[(left_right+1)%2];
		for(int ray_index=0;ray_index<source.GetCount();ray_index++)
		{
			CRay ray;
			source.Get(ray,ray_index);
			send+=ray.m_brightness;
			bool received;
			received=false;
			CRay min,min_next;
			CRoundScreen * min_receive_screen;
			bool min_receive_self;
			for(int loop=0;loop<100000;loop++)
			{
				min.m_length=1e100;

				CRay tri;
				if(loop!=0)
				{
					tri=ray;
					if(screen.Go(tri))
					{
						if(tri.m_length<min.m_length)
						{
							min=tri;
							min_next.m_brightness=-1;
							min_receive_screen=&screen;
							min_receive_self=true;
						}
					}
				}

				tri=ray;
				if(the_other_screen.Go(tri))
				{
					if(tri.m_length<min.m_length)
					{
						min=tri;
						min_next.m_brightness=-1;
						min_receive_screen=&the_other_screen;
						min_receive_self=false;
					}
				}

				tri=ray;
				CRay next;
				if(m_ellipse_mirror.Go(next,tri))
				{
					if(tri.m_length<min.m_length)
					{
						min=tri;
						min_next=next;
					}
				}

				if(min.m_length==1e100)
					ASSERT(0);
				auto Paint=[&]()
				{
					if(min_next.m_brightness==-1)
						min.Paint(-1);
					else
						min.Paint(loop);
				};
				//if(loop==0&&left_right==0&&source.m_ray_count_per_point*3<ray_index&&ray_index<=source.m_ray_count_per_point*4)
				//	min.Paint();
				//if(left_right==0&&ray_index==464)
				//	Paint();
				//if(loop==0)
				//	Paint();
				//if(min_next.m_brightness==-1)
				//	Paint();
				//if(loop<10)
				//	Paint();
				if(min_next.m_brightness==-1)
				{
					received=true;
					if(min_receive_self)
						receive+=ray.m_brightness;
					else
						the_other_receive+=ray.m_brightness;
					min_receive_screen->Receive(min,min_receive_self);
					if(loop>max_loop)
					{
						max_loop=loop;
						max_loop_side=left_right;
						max_loop_ray_index=ray_index;
					}
					break;
				}
				ray=min_next;
			}
			if(!received)
				skip+=ray.m_brightness;
		}
	}
	m_left_screen.Paint();
	m_right_screen.Paint();
	g_user_interface->m_text.Format(
		_T("Left send   %0.20f\r\n")
		_T("  receive   %0.20f\r\n")
		_T("Right send  %0.20f\r\n")
		_T("  receive   %0.20f\r\n")
		_T("Skip        %0.20f \r\n")
		_T("Send sum    %0.20f \r\n")
		_T("Receive sum %0.20f \r\n"),
		send_array[0],receive_array[0],
		send_array[1],receive_array[1],
		skip,
		send_array[0]+send_array[1],
		receive_array[0]+receive_array[1]
		);
}
