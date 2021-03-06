#include "Vis_Various.h"

VisualizationCubeWalking::VisualizationCubeWalking( int iFrequency )
{
	iProgressionX = 0;
	iProgressionY = 0;
	flSizeDataOld = 0.0f;
	flProgressionStepsXOld = 0.0f;

	m_iFrequency = iFrequency;
	if( m_iFrequency == 0 )
	{
		m_iFrequency += 1;
	}

	glGenBuffers( 1, &VBO );
}

VisualizationCubeWalking::~VisualizationCubeWalking()
{

}

void VisualizationCubeWalking::Think( DataArrayFFT fft_data )
{
	points.clear();
	float flDataNew = fft_data.Data[m_iFrequency];
	float flSizeDataNew = ( flDataNew * 0.3f ) + ( flSizeDataOld * 0.7f );
	flSizeDataOld = flSizeDataNew;
	float flStepSize = ( (float) m_iFrequency / (float) DataArraySizeFFT );

	float flFaceSizeX = 0.05f + glm::clamp( flSizeDataNew * 5.0f, 0.01f, 0.1f );
	float flFaceSizeY = 0.05f - glm::clamp( flSizeDataNew * 1.0f, 0.01f, 0.1f );

	if( flDataNew > ( 0.2f / (float) m_iFrequency ) )
		iProgressionX++;

	float flProgressionStepsX = 1.0f + ( floor( sin( (float) iProgressionX / 30.0f ) * 10.0f ) / 10.0f );
	flProgressionStepsX = ( flProgressionStepsX * 0.6f ) + ( flProgressionStepsXOld * 0.4f );
	flProgressionStepsXOld = flProgressionStepsX;
	float flProgressionStepsY = 0.025f + ( flStepSize * 2.0f );

	//flProgressionStepsX *= 2.0f;
	//flProgressionStepsY *= 2.0f;

	float flFaceOffsetX = -1.0f + flProgressionStepsX - ( flFaceSizeX / 2.0f );
	float flFaceOffsetY = 1.0f - ( flFaceSizeY / 2.0f ) - flProgressionStepsY;

	points.push_back( glm::vec3( flFaceOffsetX + -0.0f, flFaceOffsetY + -flFaceSizeY, 0.0f ) );
	points.push_back( glm::vec3( flFaceOffsetX + flFaceSizeX, flFaceOffsetY + -flFaceSizeY, 0.0f ) );
	points.push_back( glm::vec3( flFaceOffsetX + flFaceSizeX, flFaceOffsetY + flFaceSizeY, 0.0f ) );

	points.push_back( glm::vec3( flFaceOffsetX + -0.0f, flFaceOffsetY + -flFaceSizeY, 0.0f ) );
	points.push_back( glm::vec3( flFaceOffsetX + -0.0f, flFaceOffsetY + flFaceSizeY, 0.0f ) );
	points.push_back( glm::vec3( flFaceOffsetX + flFaceSizeX, flFaceOffsetY + flFaceSizeY, 0.0f ) );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, points.size() * sizeof( glm::vec3 ), &points[0], GL_DYNAMIC_DRAW );
}

void VisualizationCubeWalking::Draw()
{
	glPointSize( 2.0f );

	float colorR = 1.0f - (float) ( iProgressionX % 255 ) / 255.0f;
	float colorG = (float) ( iProgressionX % 255 ) / 255.0f;
	float colorB = (float) ( m_iFrequency / ( DataArraySizeFFT / 4 ) );

	glColor4f( colorR, colorG, colorB, flSizeDataOld );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glDrawArrays( GL_TRIANGLES, 0, points.size() );
	glDisableVertexAttribArray( 0 );
}

void VisualizationCubeWalking::Reset()
{
	iProgressionX = 0;
	iProgressionY = 0;
}